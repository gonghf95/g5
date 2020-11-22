#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <memory>

#include "src/base/Timestamp.h"

namespace net
{

class EventLoop;
class IChannelCallback;
class Channel
{
public:
	typedef std::function<void (Timestamp)> ReadEventCallback;
	typedef std::function<void ()> WriteEventCallback;

	Channel(EventLoop* loop, int fd);
	~Channel();
	
	void handleEvent();
	void setReadCallback(ReadEventCallback cb)
	{ readCallback_ = std::move(cb); }
	void setWriteCallback(WriteEventCallback cb)
	{ writeCallback_ = std::move(cb); }

	int fd() { return fd_; }
	int events() { return events_; }
	void set_revents(int events) { revents_ = events; }

	void enableReading(bool enable = true);
	void enableWriting(bool enable = true);

	bool isReading() const { return events_ & kReadEvent; }
	bool isWriting() const { return events_ & kWriteEvent; }

private:
	void update();

	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop* loop_;
	int fd_;
	int events_;
	int revents_;
	ReadEventCallback readCallback_;
	WriteEventCallback writeCallback_;
};

} // namespace net

#endif
