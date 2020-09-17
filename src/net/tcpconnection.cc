#include "tcpconnection.h"
#include "channel.h"
#include "inetcallback.h"

#include <unistd.h>
#include <iostream>

using namespace std;

TcpConnection::TcpConnection(EventLoop* loop, int fd)
 : fd_(fd),
  loop_(loop),
  callback_(NULL)
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
	char buf[1024] = {0};
	int nread = read(fd, buf, sizeof(buf));
	string msg(buf, nread);
	if(callback_ != NULL)
		callback_->onMessage(this, msg);
}

void TcpConnection::setNetCallback(INetCallback* cb)
{
	callback_ = cb;
	if(callback_ != NULL)
		callback_->onConnection(this);
}

void TcpConnection::send(const string& msg)
{
	int len = msg.length();
	int nwrite = write(fd_, msg.c_str(), len);
	if(nwrite != len)
		fprintf(stderr, "total: %d, write bytes: %d\n", len, nwrite);
}
