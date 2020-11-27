#include "src/net/TcpServer.h"
#include "src/net/EventLoop.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	net::EventLoop loop;
	net::TcpServer s(&loop);
	s.start();
	loop.loop();
	return 0;
}
