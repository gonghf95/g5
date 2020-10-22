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
	TimerQueue(EventLoop* loop);
	virtual ~TimerQueue();

	int addTimer(IRunCallback* cb, Timestamp when, int interval);

protected:
	virtual void handleRead();
	virtual void handleWrite();

private:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;

	int createTimerFd();
	void readTimerFd();
	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);

	int timerFd_;
	TimerList timers_;
	Channel* channel_;
	EventLoop* loop_;
};

#endif
