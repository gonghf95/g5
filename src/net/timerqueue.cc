#include "src/net/timerqueue.h"
#include "src/net/channel.h"
#include "src/net/eventloop.h"

#include <assert.h>
#include <sys/timerfd.h>

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop)
{
	timerFd_ = createTimerFd();

	channel_ = new Channel(loop_, timerFd_);
	channel_->setCallback(this);
	channel_->enableReading();
}

TimerQueue::~TimerQueue()
{
	delete channel_;
}

int TimerQueue::createTimerFd()
{
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
	assert(timerfd != -1);
	return timerfd;
}

void TimerQueue::handleRead()
{
}

void TimerQueue::handleWrite()
{
}
