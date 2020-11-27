#include "src/base/Timestamp.h"
#include "src/net/EventLoop.h"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	net::EventLoop event_loop;

	//Timestamp when = addTime(Timestamp::now(), 5);
	//event_loop.runAt(when, &test);

	//event_loop.runEvery(5, &test);

	//event_loop.runAfter(15, &test);

	event_loop.loop();
	return 0;
}
