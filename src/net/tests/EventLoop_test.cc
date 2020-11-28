#include "src/base/Timestamp.h"
#include "src/net/EventLoop.h"
#include "src/net/Callbacks.h"

#include <iostream>

using namespace std;

void timer_callback()
{
	cout << Timestamp::now().toFormattedString() << endl;
}

int main(int argc, char* argv[])
{
	net::TimerCallback cb = std::bind(timer_callback);

	net::EventLoop event_loop;

	Timestamp when = addTime(Timestamp::now(), 5);
	//event_loop.runAt(when, &test);

	event_loop.runEvery(5, cb);

	//event_loop.runAfter(15, &test);

	event_loop.loop();
	return 0;
}
