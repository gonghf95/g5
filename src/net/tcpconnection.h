#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "ichannelcallback.h"

class Channel;
class EventLoop;
class TcpConnection : public IChannelCallback
{
public:
	TcpConnection(EventLoop* loop, int fd);
	virtual ~TcpConnection();

	virtual void onIn(int fd);
private:
	int fd_;
	Channel* channel_;
	EventLoop* loop_;
};

#endif
