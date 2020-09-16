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

	/// inherit from parent
	virtual void onIn(int fd);

private:
	int listenfd_;
	Channel* channel_;
	EventLoop* loop_;
	IAcceptorCallback* callback_;
};

#endif
