#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "src/net/Channel.h"
#include "src/net/Callbacks.h"

#include <vector>

namespace net
{

class Epoller;
class Channel;
class TimerQueue;
class EventLoop
{
public:
	typedef std::function<void()> Functor;

	EventLoop();
	virtual ~EventLoop();

	void loop();
	void update(Channel* channel);

	void queueInLoop(Functor cb);

	int runAt(Timestamp when, TimerCallback cb);
	int runAfter(double delay, TimerCallback cb);
	int runEvery(double interval, TimerCallback cb);

private:
	void doPendingFunctors();
	void wakeup();

	void handleRead();

	bool quit_;
	Epoller* poller_;

	int eventfd_;
	Channel channel_;
	std::vector<Functor> pendingFunctors_;

	TimerQueue* timerQueue_;
};

} // namespace net

#endif
