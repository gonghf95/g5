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

TcpServer::TcpServer(EventLoop* loop)
	: loop_(loop),
	 acceptor_(new Acceptor(loop)),
	 callback_(NULL)
{
	acceptor_->setCallback(this);
}

TcpServer::~TcpServer()
{
	delete acceptor_;

	// FIXME: release memory of connections_
}

void TcpServer::start()
{
	acceptor_->start();
}

void TcpServer::newConnection(int fd)
{
	TcpConnection* conn = new TcpConnection(loop_, fd);

	if(callback_ != NULL)
		conn->setCallback(callback_);

	connections_[fd] = conn;
}
