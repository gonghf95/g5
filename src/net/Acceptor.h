#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>
#include <memory>

namespace net
{

class EventLoop;
class Channel;
class Acceptor
{
public:
	typedef function<void (int)> NewConnectionCallback;

	Acceptor(EventLoop* loop);
	~Acceptor();

	void setNewConnectionCallback(NewConnectionCallback cb)
	{ newConnectionCallback_ = std::move(cb); }

	void listen();

private:
	void handleRead();

	EventLoop* loop_;
	int listenfd_;
	Channel channel_;
	NewConnectionCallback newConnectionCallback_;
};

} // namespace net

#endif
