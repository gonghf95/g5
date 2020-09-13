#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "iacceptorcallback.h"

class Acceptor;
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
};

#endif
