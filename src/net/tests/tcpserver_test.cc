#include "src/net/tcpserver.h"
#include "src/net/eventloop.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	EventLoop loop;
	TcpServer s(&loop);
	s.start();
	loop.loop();
	return 0;
}
