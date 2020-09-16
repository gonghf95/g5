#ifndef CHANNEL_H
#define CHANNEL_H

class EventLoop;
class IChannelCallback;
class Channel
{
public:
	Channel(EventLoop* loop, int fd);
	~Channel();
	
	void setCallback(IChannelCallback* callback);
	void setRevents(int revent);
	void handleEvent();

	int fd() { return fd_; }
	int events() { return events_; }

	void enableReading();

private:
	int fd_;
	int events_;
	int revents_;
	EventLoop* loop_;
	IChannelCallback* callback_;
};

#endif
