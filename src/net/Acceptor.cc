#include "src/net/Acceptor.h"
#include "src/net/EventLoop.h"

#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>


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

using namespace net;

Acceptor::Acceptor(EventLoop* loop)
	: loop_(loop), 
	listenfd_(detail::createAndListen(kIP, kPort)),
	channel_(loop, listenfd_)
{
	channel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
	::close(listenfd_);
}

void Acceptor::handleRead()
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	int sockfd = accept(listenfd_, (struct sockaddr*)&cliaddr, &addrlen);
	assert(sockfd != -1);

	int flags = fcntl(sockfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, flags);

	newConnectionCallback_(sockfd);
}

void Acceptor::listen()
{
	channel_.enableReading();
}
