#include "src/net/timerqueue.h"
#include "src/net/channel.h"
#include "src/net/eventloop.h"

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::vector;

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop)
{
	timerFd_ = createTimerFd();

	channel_ = new Channel(loop_, timerFd_);
	channel_->setCallback(this);
	channel_->enableReading();

	addTimerWrapper_ = new TimerQueue::AddTimerWrapper(this);
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
	loop_->queueInLoop(addTimerWrapper_, timer);
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
	vector<Entry> expired;
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator end = timers_.lower_bound(sentry);
	copy(timers_.begin(), end, back_inserter(expired));
	timers_.erase(timers_.begin(), end);
	return expired;
}

void TimerQueue::reset(const vector<Entry>& expired, Timestamp now)
{
	vector<Entry>::const_iterator it;
	for(it=expired.begin(); it!=expired.end(); ++it)
	{
		if(it->second->repeat())
		{
			it->second->restart(Timestamp::now());
			insert(it->second);
		}
	}

	//
	Timestamp next_timestamp;
	if(!timers_.empty())
	{
		Timestamp next_timestamp;
		next_timestamp = timers_.begin()->second->expiration();
		if(next_timestamp.valid())
		{
			resetTimerFd(timerFd_, next_timestamp);
		}
	}
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
		cout << "TimerQueue::insert() error.\n";
	}

	return earliestChanged;
}

void TimerQueue::resetTimerFd(int timerfd, Timestamp when)
{
	struct itimerspec newVal;
	struct itimerspec oldVal;
	bzero(&newVal, sizeof(newVal));
	bzero(&oldVal, sizeof(oldVal));
	newVal.it_value = howMuchTimeFromNow(when);
	int ret = ::timerfd_settime(timerFd_, 0, &newVal, &oldVal);
	if(ret)
	{
		cout << "TimerQueue::resetTimerFd() error\n";
	}
}

struct timespec TimerQueue::howMuchTimeFromNow(Timestamp when)
{
	int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
	if(microseconds < 100)
		microseconds = 100;

	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
	return ts;
}
