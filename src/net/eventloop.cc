#include "eventloop.h"
#include "epoller.h"
#include "channel.h"
#include "iruncallback.h"
#include "timerqueue.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <assert.h>
#include <iostream>

using std::cout;
using std::vector;

namespace detail
{
int createEventFd()
{
	int fd;
	fd = eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC);
	assert(fd != -1);
	return fd;
}
} // namespace detail

EventLoop::EventLoop()
	: quit_(false),
	 poller_(new Epoller),
	 timerQueue_(new TimerQueue(this))
{
	eventfd_ = detail::createEventFd();
	channel_ = new Channel(this, eventfd_);
	channel_->setCallback(this);
	channel_->enableReading(true);
}

EventLoop::~EventLoop()
{
	delete poller_;
}

void EventLoop::loop()
{
	while(!quit_)
	{
		vector<Channel*> channels;
		poller_->poll(channels);

		vector<Channel*>::iterator it;
		for(it=channels.begin(); it!= channels.end(); it++)
		{
			(*it)->handleEvent();
		}

		doPendingFunctors();
	}
}

void EventLoop::update(Channel* channel)
{
	poller_->update(channel);
}

void EventLoop::queueInLoop(IRunCallback* cb, void* param)
{
	Runner runner(cb, param);
	pendingFunctors_.push_back(runner);
	wakeup();
}

void EventLoop::doPendingFunctors()
{
	vector<Runner> functors;
	functors.swap(pendingFunctors_);
	
	vector<Runner>::iterator it;
	for(it=functors.begin();it!=functors.end();it++)
	{
		(*it).run();
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(eventfd_, &one, sizeof(one));
	if(n != sizeof(one))
	{
		cout << "EventLoop::wakeup() writes " << n << " bytes instead of 8\n";
	}
}


void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(eventfd_, &one, sizeof(one));
	if (n != sizeof(one))
	{
		cout  << "EventLoop::handleRead() reads " << n << " bytes instead of 8\n";
	}
}

void EventLoop::handleWrite()
{

}

int EventLoop::runAt(Timestamp when, IRunCallback* cb)
{
	return timerQueue_->addTimer(cb, when, 0.0);
}

int EventLoop::runAfter(double delay, IRunCallback* cb)
{
	Timestamp when = addTime(Timestamp::now(), delay);
	return timerQueue_->addTimer(cb, when, 0.0);
}

int EventLoop::runEvery(double interval, IRunCallback* cb)
{
	Timestamp when = addTime(Timestamp::now(), interval);
	return timerQueue_->addTimer(cb, when, interval);
}
