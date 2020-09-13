#ifndef ICHANNELCALLBACK_H
#define ICHANNELCALLBACK_H

class IChannelCallback
{
public:
	virtual void onIn(int fd) = 0;
};

#endif
