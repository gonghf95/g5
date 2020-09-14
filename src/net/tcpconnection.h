#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "ichannelcallback.h"

class Channel;
class TcpConnection : public IChannelCallback
{
public:
	TcpConnection(int epfd, int fd);
	virtual ~TcpConnection();

	virtual void onIn(int fd);
private:
	int epfd_;
	int fd_;

	Channel* channel_;
};

#endif
