#include "src/net/timerqueue.h"
#include "src/net/channel.h"
#include "src/net/eventloop.h"

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::vector;

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

int TimerQueue::addTimer(IRunCallback* cb, Timestamp when, int interval)
{
	Timer* timer = new Timer(when, interval, cb);
	loop_->queueInLoop(cb, timer);
	//return timer;
	return -1;  // FIXME: 
}

void TimerQueue::handleRead()
{
	// read bytes
	readTimerFd();

	// get expired timer and run the callback
	Timestamp now(Timestamp::now());
	vector<TimerQueue::Entry> expired = getExpired(now);
	vector<TimerQueue::Entry>::iterator it;
	for(it = expired.begin(); it != expired.end(); ++it)
	{
		it->second->run();
	}

	// reset the timer
	reset(expired, now);
}

void TimerQueue::handleWrite()
{
}

void TimerQueue::readTimerFd()
{
	uint64_t one = 1;
	size_t n = ::read(timerFd_, &one, sizeof(one));
	if(n != sizeof(one))
	{
		cout << "TimerQueue::handleRead() reads " << n << " bytes instead of 8\n";
	}
}

vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{

	return {};
}

void TimerQueue::reset(const vector<Entry>& expired, Timestamp now)
{

}

void TimerQueue::doAddTimer(void* param)
{
	Timer* timer = static_cast<Timer*>(param);

	bool earliestChanged = insert(timer);
	if(earliestChanged)
	{
		resetTimerFd(timerFd_, timer->expiration());
	}
}

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	
	Timestamp when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if(it==timers_.end() || when < it->first)
	{
		earliestChanged = true;
	}

	std::pair<TimerList::iterator, bool> result = timers_.insert(Entry(when, timer));
	if(!result.second)
	{
		cout << "TimerQueue::insert error.\n";
	}

	return earliestChanged;
}

void TimerQueue::resetTimerFd(int timerfd, Timestamp when)
{

}
