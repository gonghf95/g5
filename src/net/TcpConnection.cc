#include "src/net/TcpConnection.h"
#include "src/net/Channel.h"
#include "src/net/EventLoop.h"

#include <unistd.h>
#include <string.h>

using std::string;
using namespace net;

void net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
	cout << "new connection\n";
}

void net::defaultMessageCallback(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
	cout << buf->retrieveAllAsString();
}

TcpConnection::TcpConnection(EventLoop* loop, int fd)
	: loop_(loop),
	fd_(fd),
	channel_(loop_, fd_)
{
	channel_.setReadCallback(std::bind(&TcpConnection::handleRead, this));
	channel_.setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
}

TcpConnection::~TcpConnection()
{
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

	inputBuf_.append(string(buf, nread));

	messageCallback_(shared_from_this(), &inputBuf_, Timestamp::now());
}

void TcpConnection::handleWrite()
{
	if(channel_.isWriting())
	{
		int nwrite = write(fd_, outputBuf_.peek(), outputBuf_.readableBytes());
		if(nwrite > 0)
		{
			outputBuf_.retrieve(nwrite);
			if(outputBuf_.readableBytes() <= 0)
			{
				channel_.enableWriting(false);
				loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
			}
		}
	}
}

void TcpConnection::send(const string& msg)
{
	int nwrite = 0;
	if(outputBuf_.readableBytes() <= 0)
	{
		nwrite = write(fd_, msg.c_str(), msg.length());
		if(nwrite == -1)
		{
			fprintf(stderr, "err: %d %s\n", errno, strerror(errno));
		}
		else if((nwrite == static_cast<int>(msg.length())) && writeCompleteCallback_)
		{
			loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
		}
	}

	if(nwrite < static_cast<int>(msg.length()))
	{
		outputBuf_.append(msg.substr(nwrite, msg.length()));
		if(!channel_.isWriting())
			channel_.enableWriting(true);
	}
}

void TcpConnection::connectEstablished()
{
	channel_.enableReading();

	connectionCallback_(shared_from_this());
}
