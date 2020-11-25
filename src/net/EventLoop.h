#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "ichannelcallback.h"
#include "iruncallback.h"
#include "src/base/Timestamp.h"

#include <vector>

class Epoller;
class Channel;
class TimerQueue;
class EventLoop : public IChannelCallback
{
public:

	class Runner
	{
	public:
		Runner(IRunCallback* cb, void* param)
			: callback_(cb),
			param_(param)
		{
		}

		void run()
		{
			callback_->run(param_);
		}
	private:
		IRunCallback* callback_;
		void* param_;
	};

	EventLoop();
	virtual ~EventLoop();

	void loop();
	void update(Channel* channel);

	void queueInLoop(IRunCallback* cb, void* param);

	int runAt(Timestamp when, IRunCallback* cb);
	int runAfter(double delay, IRunCallback* cb);
	int runEvery(double interval, IRunCallback* cb);

private:
	void doPendingFunctors();
	void wakeup();

	void handleRead();
	void handleWrite();

	bool quit_;
	Epoller* poller_;
	int eventfd_;
	Channel* channel_;
	std::vector<Runner> pendingFunctors_;
	TimerQueue* timerQueue_;
};

#endif
