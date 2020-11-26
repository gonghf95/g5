#include "src/net/TimerQueue.h"
#include "src/net/EventLoop.h"

#include <assert.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;
using std::vector;

namespace net
{

namespace detail
{

int createTimerfd()
{
	int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
	assert(timerfd != -1);
	return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
	int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
	if(microseconds < 100)
		microseconds = 100;

	struct timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
	ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
	return ts;
}

void readTimerfd(int timerfd)
{
	uint64_t one = 1;
	size_t n = ::read(timerfd, &one, sizeof(one));
	if(n != sizeof(one))
	{
		cout << "TimerQueue::handleRead() reads " << n << " bytes instead of 8\n";
	}
}

void resetTimerfd(int timerfd, Timestamp when)
{
	struct itimerspec newVal;
	struct itimerspec oldVal;
	bzero(&newVal, sizeof(newVal));
	bzero(&oldVal, sizeof(oldVal));
	newVal.it_value = howMuchTimeFromNow(when);
	int ret = ::timerfd_settime(timerfd, 0, &newVal, &oldVal);
	if(ret)
	{
		cout << "TimerQueue::resetTimerFd() error\n";
	}
}

} // namespace detail

} // namespace net

using namespace net;
using namespace net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop),
	timerfd_(createTimerfd()),
	channel_(loop_, timerfd_)
{
	channel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
	channel_.enableReading();
}

TimerQueue::~TimerQueue()
{
	close(timerfd_);
}

int TimerQueue::addTimer(TimerCallback cb, Timestamp when, int interval)
{
	Timer* timer = new Timer(std::move(cb), when, interval);
	loop_->queueInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
	return -1; // FIXME:
}

void TimerQueue::handleRead()
{
	readTimerfd(timerfd_);

	Timestamp now(Timestamp::now());
	vector<TimerQueue::Entry> expired = getExpired(now);
	vector<TimerQueue::Entry>::iterator it;
	for(it = expired.begin(); it != expired.end(); ++it)
	{
		it->second->run();
	}

	reset(expired, now);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
	bool earliestChanged = insert(timer);
	if(earliestChanged)
	{
		resetTimerfd(timerfd_, timer->expiration());
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
			resetTimerfd(timerfd_, next_timestamp);
		}
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
