#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "iacceptorcallback.h"

#include <map>

class Acceptor;
class TcpConnection;
class TcpServer : public IAcceptorCallback
{
public:
	TcpServer();
	~TcpServer();

	void start();

	/// inherit from IChannelCallback
	void newConnection(int fd);

private:
	int epfd_;
	Acceptor* acceptor_;

	std::map<int, TcpConnection*> connections_;
};

#endif
