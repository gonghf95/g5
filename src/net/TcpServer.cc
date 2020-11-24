#include "tcpserver.h"
#include "acceptor.h"
#include "channel.h"
#include "tcpconnection.h"

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
	acceptor_.setNewConnectionCallback(std::bind(&TcpServer::newConnection, this));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
	acceptor_.listen();
}

void TcpServer::newConnection(int fd)
{
	TcpConnectionPtr conn(new TcpConnection(loop_, sockfd));
	conn->setMessageCallback(messageCallback_);
	
	connections_[fd] = conn;
}

} // namespace net
