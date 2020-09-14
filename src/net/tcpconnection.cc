#include "tcpconnection.h"
#include "channel.h"

#include <unistd.h>
#include <iostream>

using namespace std;

TcpConnection::TcpConnection(int epfd, int fd)
 : epfd_(epfd),
  fd_(fd)
{
	channel_ = new Channel(epfd_, fd_);
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
	cout << buf << endl;
}
