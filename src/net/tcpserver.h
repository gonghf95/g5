#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "iacceptorcallback.h"

#include <map>

class Acceptor;
class TcpConnection;
class EventLoop;
class TcpServer : public IAcceptorCallback
{
public:
	TcpServer(EventLoop* loop);
	~TcpServer();

	void start();

	/// inherit from IChannelCallback
	void newConnection(int fd);

private:
	EventLoop* loop_;
	Acceptor* acceptor_;

	std::map<int, TcpConnection*> connections_;
};

#endif
