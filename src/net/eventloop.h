#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "ichannelcallback.h"

#include <vector>

class Epoller;
class Channel;
class IRunCallback;
class EventLoop : public IChannelCallback
{
public:
	EventLoop();
	virtual ~EventLoop();

	void loop();
	void update(Channel* channel);

	void queueInLoop(IRunCallback* cb);

private:
	void doPendingFunctors();
	void wakeup();

	void handleRead();
	void handleWrite();

	bool quit_;
	Epoller* poller_;
	int eventfd_;
	Channel* channel_;
	std::vector<IRunCallback*> pendingFunctors_;
};

#endif
