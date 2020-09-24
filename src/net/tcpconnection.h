#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "buffer.h"
#include "ichannelcallback.h"
#include "iruncallback.h"

#include <string>

class Channel;
class EventLoop;
class INetCallback;
class TcpConnection : public IChannelCallback, public IRunCallback
{
public:
	TcpConnection(EventLoop* loop, int fd);
	virtual ~TcpConnection();

	void setCallback(INetCallback* cb);

	void send(const std::string& msg);

protected:
	virtual void handleRead();
	virtual void handleWrite();
	virtual void run();

private:
	int fd_;
	Channel* channel_;
	INetCallback* callback_;
	EventLoop* loop_;

	Buffer inBuf_;
	Buffer outBuf_;
};

#endif
