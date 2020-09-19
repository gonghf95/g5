#include "acceptor.h"
#include "channel.h"
#include "iacceptorcallback.h"
#include "ichannelcallback.h"
#include "eventloop.h"
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>


const char kIP[] = "127.0.0.1";
const int kPort = 5188;

namespace detail
{

int createAndListen(const char* ip, int port)
{
	int listenfd;
	listenfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(listenfd != -1);

	int use = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(use));

	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	// svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int ret;
	ret = bind(listenfd, (struct sockaddr*)&svraddr, sizeof(svraddr));
	assert(ret != -1);

	ret = listen(listenfd, SOMAXCONN);
	assert(ret != -1);

	return listenfd;
}

} // namespace detail

Acceptor::Acceptor(EventLoop* loop)
	: listenfd_(-1), 
	 channel_(NULL),
	 callback_(NULL),
	 loop_(loop)
{
}

Acceptor::~Acceptor()
{
	if(channel_ != NULL)
		delete channel_;
	if(callback_ != NULL)
		delete callback_;
	channel_ = NULL;
	callback_ = NULL;
}

void Acceptor::handleRead()
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	int connfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &addrlen);
	assert(connfd != -1);

	//
	// set nonblocking
	int flags = fcntl(connfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(connfd, F_SETFL, flags);

	if(callback_!=NULL)
		callback_->newConnection(connfd);
}

void Acceptor::handleWrite()
{
}

void Acceptor::start()
{
	listenfd_ = detail::createAndListen(kIP, kPort);
	channel_ = new Channel(loop_, listenfd_);
	channel_->setCallback(this);
	channel_->enableReading();
}
