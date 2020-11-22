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

	void newConnection(int fd);

private:
	typedef shared_ptr<Acceptor> AcceptorPtr;

	EventLoop* loop_;
	Acceptor* acceptor_;
	INetCallback* callback_;

	std::map<int, TcpConnection*> connections_;
};

#endif
