#ifndef IACCEPTORCALLBACK_H
#define IACCEPTORCALLBACK_H

class IAcceptorCallback
{
public:
	virtual ~IAcceptorCallback() = default;
	virtual void newConnection(int fd) = 0;
};

#endif
