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
	inBuf_.append(string(buf, nread));
	if(callback_ != NULL)
		callback_->onMessage(this, inBuf_);
}

void TcpConnection::handleWrite()
{
	if(channel_->writable())
	{
		int nwrite = write(fd_, outBuf_.peek(), outBuf_.readableBytes());
		if(nwrite > 0)
		{
			outBuf_.retrieve(nwrite);
			if(outBuf_.readableBytes() <= 0)
			{
				channel_->enableWriting(false);
			}
		}
	}
}

void TcpConnection::setCallback(INetCallback* cb)
{
	callback_ = cb;
	if(callback_ != NULL)
		callback_->onConnection(this);
}

void TcpConnection::send(const string& msg)
{
	int nwrite = 0;
	if(outBuf_.readableBytes() <= 0)
	{
		nwrite = write(fd_, msg.c_str(), msg.length());
		if(nwrite == -1)
			fprintf(stderr, "err: %d %s\n", errno, strerror(errno));
	}

	if(nwrite < static_cast<int>(msg.length()))
	{
		outBuf_.append(msg.substr(nwrite, msg.length()));
		if(!channel_->writable())
			channel_->enableWriting(true);
	}
}
