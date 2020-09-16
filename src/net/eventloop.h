#ifndef EVENTLOOP_H
#define EVENTLOOP_H

class Epoller;
class Channel;
class EventLoop
{
public:
	EventLoop();
	virtual ~EventLoop();

	void loop();
	void update(Channel* channel);

private:
	bool quit_;
	Epoller* poller_;
};

#endif
