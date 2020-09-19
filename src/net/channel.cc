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
		callback_->handleRead();
	if(revents_ & EPOLLOUT)
		callback_->handleWrite();
}

void Channel::enableReading(bool enable)
{
	if(enable)
		events_ |= EPOLLIN;
	else
		events_ &= ~EPOLLIN;

	update();
}

void Channel::enableWriting(bool enable)
{
	if(enable)
		events_ |= EPOLLOUT;
	else
		events_ &= ~EPOLLOUT;

	update();
}

void Channel::update()
{
	loop_->update(this);
}

bool Channel::writable()
{
	return events_ & EPOLLOUT;
}

