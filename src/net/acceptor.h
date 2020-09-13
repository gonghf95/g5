#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "ichannelcallback.h"

class Channel;
class IAcceptorCallback;
class Acceptor : public IChannelCallback
{
public:
	Acceptor(int epfd);
	virtual ~Acceptor();

	void setCallback(IAcceptorCallback* cb) { this->callback_ = cb; }

	void listen();

	/// inherit from parent
	virtual void onIn(int fd);

private:
	int epfd_;
	int listenfd_;
	Channel* channel_;
	IAcceptorCallback* callback_;
};

#endif
