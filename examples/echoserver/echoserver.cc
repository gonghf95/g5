#include "echoserver.h"
#include "src/net/eventloop.h"
#include "src/net/tcpserver.h"
#include "src/net/tcpconnection.h"

#include <iostream>

using std::cout;
using std::endl;

EchoServer::EchoServer()
	: loop_(new EventLoop),
	 tcpServer_(new TcpServer(loop_))
{
	tcpServer_->setNetCallback(this);
}

EchoServer::~EchoServer()
{
	delete tcpServer_;
	delete loop_;
}

void EchoServer::start()
{
	tcpServer_->start();
	loop_->loop();
}

void EchoServer::onConnection(TcpConnection* conn)
{
	cout << "new connection\n";
}

void EchoServer::onMessage(TcpConnection* conn, Buffer& buf)
{
	string msg = buf.retrieveAllAsString();
	cout << msg << endl;
	conn->send(msg);
}

void EchoServer::onWriteComplete()
{
	cout << "write complete\n";
}
