#include "tcpconnection.h"
#include "channel.h"

#include <unistd.h>
#include <iostream>

using namespace std;

TcpConnection::TcpConnection(EventLoop* loop, int fd)
 : fd_(fd),
 loop_(loop)
{
	channel_ = new Channel(loop_, fd_);
	channel_->setCallback(this);
	channel_->enableReading();
}

TcpConnection::~TcpConnection()
{
	delete channel_;
}

void TcpConnection::onIn(int fd)
{
	cout << "TcpConnection::onIn() receive data, but not read.\n";
	char buf[1024] = {0};
	int nread = read(fd, buf, sizeof(buf));
	cout << "receive bytes: "<< nread << ", "<< buf << endl;
}
