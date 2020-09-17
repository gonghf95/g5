#ifndef INETCALLBACK_H
#define INETCALLBACK_H

#include <string>

class TcpConnection;
class INetCallback
{
public:
	virtual void onConnection(TcpConnection* conn) = 0;
	virtual void onMessage(TcpConnection* conn, const std::string& msg) = 0;
};

#endif
