#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "src/net/inetcallback.h"

#include <string>

using std::string;

class EventLoop;
class TcpServer;
class TcpConnection;
class EchoServer : public INetCallback
{
public:
	EchoServer();
	virtual ~EchoServer();

	void start();

protected:
	void onConnection(TcpConnection* conn);
	void onMessage(TcpConnection* conn, const string& msg);

private:
	EventLoop* loop_;
	TcpServer* tcpServer_;
};

#endif
