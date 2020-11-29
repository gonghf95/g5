#ifndef ECHOSERVER_H
#define ECHOSERVER_H

#include "src/net/EventLoop.h"
#include "src/net/TcpServer.h"
#include "src/net/Callbacks.h"

using namespace net;

class EchoServer
{
public:
	explicit EchoServer(EventLoop* loop);
	virtual ~EchoServer();

	void start();

private:
	void onConnection(const net::TcpConnectionPtr& conn);
	void onMessage(const net::TcpConnectionPtr& conn, net::Buffer* msg, Timestamp when);

	EventLoop* loop_;
	TcpServer* tcpServer_;
};

#endif
