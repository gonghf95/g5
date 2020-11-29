#include "src/net/TcpServer.h"
#include "src/net/Channel.h"
#include "src/net/TcpConnection.h"
#include "src/net/EventLoop.h"

using namespace net;

TcpServer::TcpServer(EventLoop* loop)
	: loop_(loop),
	acceptor_(loop),
	connectionCallback_(defaultConnectionCallback),
	messageCallback_(defaultMessageCallback)
{
	acceptor_.setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
	acceptor_.listen();
}

void TcpServer::newConnection(int sockfd)
{
	TcpConnectionPtr conn(new TcpConnection(loop_, sockfd));

	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	
	connections_[sockfd] = conn;

	loop_->queueInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}
