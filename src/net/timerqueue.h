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

protected:
	virtual void handleRead();
	virtual void handleWrite();

private:
	typedef std::pair<Timestamp, Timer*> Entry;
	typedef std::set<Entry> TimerList;

	int createTimerFd();

	int timerFd_;
	Channel* channel_;
	EventLoop* loop_;
};

#endif
