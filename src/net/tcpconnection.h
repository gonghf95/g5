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

	void setCallback(INetCallback* cb);

	void send(const std::string& msg);

protected:
	virtual void handleRead();
	virtual void handleWrite();

private:
	int fd_;
	Channel* channel_;
	INetCallback* callback_;
	EventLoop* loop_;

	std::string inBuf_;
	std::string outBuf_;
};

#endif
