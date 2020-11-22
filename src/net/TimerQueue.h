#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "src/net/ichannelcallback.h"
#include "src/net/Timer.h"
#include "src/base/timestamp.h"
#include "src/net/Callbacks.h"

#include <vector>
#include <set>

namespace net
{

class Channel;
class EventLoop;
class TimerQueue : public IChannelCallback
{
public:
	TimerQueue(EventLoop* loop);
	virtual ~TimerQueue();

	int addTimer(TimerCallback cb, Timestamp when, int interval);

private:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;

	void handleRead();

	void addTimerInLoop(Timer* timer);

	std::vector<Entry> getExpired(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);
	bool insert(Timer* timer);

	EventLoop* loop_;
	int timerfd_;
	Channel* channel_;
	TimerList timers_;
};

} // namespace net

#endif
