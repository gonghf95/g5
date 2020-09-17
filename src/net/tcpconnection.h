#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "ichannelcallback.h"

#include <string>

class Channel;
class EventLoop;
class INetCallback;
class TcpConnection : public IChannelCallback
{
public:
	TcpConnection(EventLoop* loop, int fd);
	virtual ~TcpConnection();

	void setNetCallback(INetCallback* cb);

	void send(const std::string& msg);

protected:
	virtual void onIn(int fd);

private:
	int fd_;
	Channel* channel_;
	EventLoop* loop_;
	INetCallback* callback_;
};

#endif
