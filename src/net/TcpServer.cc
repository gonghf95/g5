#include "src/net/TcpServer.h"
#include "src/net/Channel.h"
#include "src/net/TcpConnection.h"

#include <string.h>
#include <assert.h>
#include <sys/epoll.h>
#include <vector>
#include <iostream>

#define MAXEVENTS 1024

using namespace std;

namespace net
{

TcpServer::TcpServer(EventLoop* loop)
	: loop_(loop),
	acceptor_(loop)
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
	conn->setMessageCallback(messageCallback_);
	
	connections_[sockfd] = conn;
}

} // namespace net
