#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "src/net/ichannelcallback.h"
#include "src/net/timer.h"
#include "src/base/timestamp.h"

#include <vector>
#include <set>

class Channel;
class EventLoop;
class TimerQueue : public IChannelCallback
{
public:
	class AddTimerWrapper : public IRunCallback
	{
	public:
		AddTimerWrapper(TimerQueue* queue)
			: timerQueue_(queue)
		{
		}

		void run(void* param)
		{
			timerQueue_->doAddTimer(param);
		}
	private:
		TimerQueue* timerQueue_;
	};

	TimerQueue(EventLoop* loop);
	virtual ~TimerQueue();

	int addTimer(IRunCallback* cb, Timestamp when, int interval);

private:
	friend class AddTimerWrapper;

	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;

	void handleRead();
	void handleWrite();

	int createTimerFd();
	void readTimerFd();
	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);
	void doAddTimer(void* timer);
	bool insert(Timer* timer);
	void resetTimerFd(int timerfd, Timestamp when);

	int timerFd_;
	TimerList timers_;
	Channel* channel_;
	EventLoop* loop_;
	AddTimerWrapper* addTimerWrapper_;
};

#endif
