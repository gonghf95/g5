#ifndef INETCALLBACK_H
#define INETCALLBACK_H

#include <string>

class TcpConnection;
class Buffer;
class INetCallback
{
public:
	virtual void onConnection(TcpConnection* conn) = 0;
	virtual void onMessage(TcpConnection* conn, Buffer& buf) = 0;
	virtual void onWriteComplete() = 0;
};

#endif
