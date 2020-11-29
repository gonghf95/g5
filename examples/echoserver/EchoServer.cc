#include "EchoServer.h"
#include "src/net/Buffer.h"
#include "src/net/TcpConnection.h"

#include <iostream>

using std::cout;
using std::endl;
using namespace net;

EchoServer::EchoServer(EventLoop* loop)
	: loop_(loop),
	tcpServer_(new TcpServer(loop_))
{
	tcpServer_->setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
	tcpServer_->setMessageCallback(std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

EchoServer::~EchoServer()
{
	delete tcpServer_;
}

void EchoServer::start()
{
	tcpServer_->start();
}

void EchoServer::onConnection(const TcpConnectionPtr& conn)
{
	cout << "new connection\n";
}

void EchoServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
	string msg = buf->retrieveAllAsString();
	cout << msg << endl;
	conn->send(msg);
}
