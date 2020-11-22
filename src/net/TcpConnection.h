#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Buffer.h"
#include <string>

namespace net
{

class Channel;
class EventLoop;
class TcpConnection
{
public:
	TcpConnection(EventLoop* loop, int fd);
	virtual ~TcpConnection();

	void setCallback(INetCallback* cb);

	void send(const std::string& msg);

protected:
	virtual void handleRead();
	virtual void handleWrite();
	virtual void run(void* param);

private:
	int fd_;
	Channel* channel_;
	INetCallback* callback_;
	EventLoop* loop_;

	Buffer inBuf_;
	Buffer outBuf_;
};

} // namespace net

#endif
