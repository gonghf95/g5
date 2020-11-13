#include "src/base/timestamp.h"
#include "src/net/iruncallback.h"
#include "src/net/eventloop.h"

#include <iostream>

using namespace std;

class Test : public IRunCallback
{
public:
	void run(void* args)
	{
		cout <<  Timestamp::now().toFormattedString() << endl;
	}
};

int main(int argc, char* argv[])
{
	EventLoop event_loop;

	Test test;
	Timestamp when = addTime(Timestamp::now(), 5);
	event_loop.runAt(when, &test);

	event_loop.runEvery(5, &test);

	event_loop.runAfter(15, &test);

	event_loop.loop();
	return 0;
}
