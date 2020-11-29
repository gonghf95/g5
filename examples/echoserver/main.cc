#include "src/net/EventLoop.h"
#include "EchoServer.h"

int main(int argc, char* argv[])
{
	net::EventLoop loop;
	EchoServer s(&loop);
	s.start();
	loop.loop();
	return 0;
}
