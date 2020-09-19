#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "ichannelcallback.h"

class EventLoop;
class Channel;
class IAcceptorCallback;
class Acceptor : public IChannelCallback
{
public:
	Acceptor(EventLoop* loop);
	virtual ~Acceptor();

	void setCallback(IAcceptorCallback* cb) { this->callback_ = cb; }

	void start();

protected:
	virtual void handleRead();

private:
	int listenfd_;
	Channel* channel_;
	IAcceptorCallback* callback_;
	EventLoop* loop_;
};

#endif
