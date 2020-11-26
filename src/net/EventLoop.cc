#include "src/net/EventLoop.h"
#include "src/net/Epoller.h"
#include "src/net/Channel.h"
#include "src/net/TimerQueue.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <assert.h>
#include <iostream>

using std::cout;
using std::vector;

namespace net
{

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

} // namespace net

using namespace net;
using namespace net::detail;

EventLoop::EventLoop()
	: quit_(false),
	 poller_(new Epoller),
	 eventfd_(detail::createEventFd()),
	 channel_(this, eventfd_),
	 timerQueue_(new TimerQueue(this))
{
	channel_.setReadCallback(std::bind(&EventLoop::handleRead, this));
	channel_.enableReading();
}

EventLoop::~EventLoop()
{
	close(eventfd_);

	delete poller_;
	delete timerQueue_;
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

void EventLoop::queueInLoop(Functor cb)
{
	pendingFunctors_.push_back(std::move(cb));
	wakeup();
}

void EventLoop::doPendingFunctors()
{
	vector<Functor> functors;
	functors.swap(pendingFunctors_);
	
	vector<Functor>::iterator it;
	for(it=functors.begin();it!=functors.end();it++)
	{
		(*it)();
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

int EventLoop::runAt(Timestamp when, TimerCallback cb)
{
	return timerQueue_->addTimer(cb, when, 0.0);
}

int EventLoop::runAfter(double delay, TimerCallback cb)
{
	Timestamp when = addTime(Timestamp::now(), delay);
	return timerQueue_->addTimer(cb, when, 0.0);
}

int EventLoop::runEvery(double interval, TimerCallback cb)
{
	Timestamp when = addTime(Timestamp::now(), interval);
	return timerQueue_->addTimer(cb, when, interval);
}
