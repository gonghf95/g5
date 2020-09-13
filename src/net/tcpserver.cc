#include "tcpserver.h"
#include "acceptor.h"
#include "channel.h"

#include <string.h>
#include <assert.h>
#include <sys/epoll.h>
#include <vector>
#include <iostream>

#define MAXEVENTS 1024

using namespace std;

TcpServer::TcpServer()
{
	epfd_ = epoll_create1(0);
	assert(epfd_ != -1);
	
	acceptor_ = new Acceptor(epfd_);
	acceptor_->setCallback(this);
}

TcpServer::~TcpServer()
{
	delete acceptor_;
}

void TcpServer::start()
{
	acceptor_->listen();

	struct epoll_event events[MAXEVENTS];
	while(true)
	{
		int nready = epoll_wait(epfd_, events, MAXEVENTS, -1);

		vector<Channel*> channels;
		for(int i=0; i<nready; ++i)
		{
			Channel* ch = static_cast<Channel*>(events[i].data.ptr);
			ch->setRevents(events[i].events);
			channels.push_back(ch);
		}

		vector<Channel*>::iterator it;
		for(it=channels.begin(); it!=channels.end(); it++)
		{
			(*it)->handleEvent();
		}
	}
}

void TcpServer::newConnection(int fd)
{
	cout << "new connection: " << fd << endl;
}
