#include "src/net/Epoller.h"
#include "src/net/Channel.h"

#include <assert.h>

using std::vector;

namespace net
{

Epoller::Epoller()
{
	epfd_ = epoll_create1(0);
	assert(epfd_ != -1);
}

Epoller::~Epoller()
{
}

void Epoller::poll(vector<Channel*>& channels)
{
	int nready = epoll_wait(epfd_, events_, MAXEVENTS, -1);
	for(int i=0; i<nready; ++i)
	{
		Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
		ch->set_revents(events_[i].events);
		channels.push_back(ch);
	}
}

void Epoller::update(Channel* channel)
{
	struct epoll_event ev;
	ev.data.ptr = channel;
	ev.events = channel->events();
	int fd = channel->fd();
	epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
}

} // namespace net
