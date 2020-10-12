#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "src/net/ichannelcallback.h"

class Channel;
class EventLoop;
class TimerQueue : public IChannelCallback
{
public:
	TimerQueue(EventLoop* loop);
	virtual ~TimerQueue();

	void handleRead();
	void handleWrite();

private:
	int createTimerFd();

	int timerFd_;
	Channel* channel_;
	EventLoop* loop_;
};

#endif
