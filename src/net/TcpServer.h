#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "src/net/Callbacks.h"

#include <map>

namespace net
{

class Acceptor;
class TcpConnection;
class EventLoop;
class TcpServer
{
public:
	TcpServer(EventLoop* loop);
	~TcpServer();

	void start();

	void setConnectionCallback(ConnectionCallback cb)
	{ connectionCallback_ = std::move(cb); }

	void setMessageCallback(MessageCallback cb)
	{ messageCallback_ = std::move(cb); }

private:
	typedef shared_ptr<Acceptor> AcceptorPtr;
	typedef std::map<int, TcpConnection*> ConnectionMap;

	void newConnection(int sockfd);

	EventLoop* loop_;
	Acceptor acceptor_;

	ConnectionMap connections_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
};

} // namespace net

#endif
