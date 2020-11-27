#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "src/net/Buffer.h"
#include "src/net/Channel.h"
#include "src/net/Callbacks.h"
#include <string>

namespace net
{

class Channel;
class EventLoop;
class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(EventLoop* loop, int fd);
	virtual ~TcpConnection();

	void send(const std::string& msg);

	void setConnectionCallback(ConnectionCallback cb)
	{ connectionCallback_ = std::move(cb); }
	void setCloseCallback(CloseCallback cb)
	{ closeCallback_ = std::move(cb); }
	void setWriteCompleteCallback(WriteCompleteCallback cb)
	{ writeCompleteCallback_ = std::move(cb); }
	void setMessageCallback(MessageCallback cb)
	{ messageCallback_ = std::move(cb); }

private:
	void handleRead();
	void handleWrite();

	EventLoop* loop_;
	int fd_;
	Channel channel_;

	Buffer inputBuf_;
	Buffer outputBuf_;

	ConnectionCallback connectionCallback_;
	CloseCallback closeCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	MessageCallback messageCallback_;
};

} // namespace net

#endif
