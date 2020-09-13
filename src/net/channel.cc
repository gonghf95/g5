#include "channel.h"
#include "ichannelcallback.h"
#include <sys/epoll.h>

Channel::Channel(int epollfd, int fd)
	: epollfd_(epollfd),
	fd_(fd)
{
}

Channel::~Channel()
{
}

void Channel::setCallback(IChannelCallback* cb)
{
	callback_ = cb;
}

void Channel::setRevents(int events)
{
	revents_ = events;
}

void Channel::handleEvent()
{
	if(revents_ & EPOLLIN)
		callback_->onIn(fd_);
}

void Channel::enableReading()
{
	events_ |= EPOLLIN;

	struct epoll_event ev;
	ev.data.ptr = this;
	ev.events = events_;
	epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd_, &ev);
}
