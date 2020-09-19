#include "tcpconnection.h"
#include "channel.h"
#include "inetcallback.h"

#include <unistd.h>
#include <string.h>

using std::string;

TcpConnection::TcpConnection(EventLoop* loop, int fd)
 : fd_(fd),
 callback_(NULL),
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

void TcpConnection::handleRead()
{
	char buf[1024] = {0};
	int nread = read(fd_, buf, sizeof(buf));
	if(nread <= 0)
	{
		if(nread == 0)
		{
			close(fd_);
		}
		else if(nread == -1)
		{
			fprintf(stderr, "err: %s\n", strerror(errno));
		}
		return;
	}
	string msg(buf, nread);
	if(callback_ != NULL)
		callback_->onMessage(this, msg);
}

void TcpConnection::setCallback(INetCallback* cb)
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
