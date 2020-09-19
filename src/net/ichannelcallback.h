#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallback
{
public:
	virtual void handleRead() = 0;
};

#endif
