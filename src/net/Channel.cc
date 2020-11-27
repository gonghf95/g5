#include "src/net/Channel.h"
#include "src/net/EventLoop.h"
#include <sys/epoll.h>

namespace net
{

const int Channel::kReadEvent = EPOLLIN;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop* loop, int fd)
	: loop_(loop),
	fd_(fd),
	events_(0),
	revents_(0)
{
}

Channel::~Channel()
{
}

void Channel::handleEvent()
{
	if(revents_ & kReadEvent)
		readCallback_();

	if(revents_ & kWriteEvent)
		writeCallback_();
}

void Channel::enableReading(bool enable)
{
	if(enable)
		events_ |= kReadEvent;
	else
		events_ &= ~kReadEvent;

	update();
}

void Channel::enableWriting(bool enable)
{
	if(enable)
		events_ |= kWriteEvent;
	else
		events_ &= ~kWriteEvent;

	update();
}

void Channel::update()
{
	loop_->update(this);
}

} // namespace net
