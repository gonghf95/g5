#include "channel.h"
#include "ichannelcallback.h"
#include "eventloop.h"
#include <sys/epoll.h>

Channel::Channel(EventLoop* loop, int fd)
	: fd_(fd),
	 loop_(loop)
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

	loop_->update(this);
}
