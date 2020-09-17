#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "iacceptorcallback.h"

#include <map>

class Acceptor;
class TcpConnection;
class EventLoop;
class INetCallback;
class TcpServer : public IAcceptorCallback
{
public:
	TcpServer(EventLoop* loop);
	~TcpServer();

	void start();

	void setNetCallback(INetCallback* cb) { callback_ = cb; }

	/// inherit from IChannelCallback
	void newConnection(int fd);

private:
	EventLoop* loop_;
	Acceptor* acceptor_;
	INetCallback* callback_;

	std::map<int, TcpConnection*> connections_;
};

#endif
