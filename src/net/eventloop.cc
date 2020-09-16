#include "eventloop.h"
#include "epoller.h"
#include "channel.h"

#include <vector>

using std::vector;

EventLoop::EventLoop()
	: quit_(false),
	 poller_(new Epoller)
{
}

EventLoop::~EventLoop()
{
	delete poller_;
}

void EventLoop::loop()
{
	while(!quit_)
	{
		vector<Channel*> channels;
		poller_->poll(channels);

		vector<Channel*>::iterator it;
		for(it=channels.begin(); it!= channels.end(); it++)
			(*it)->handleEvent();
	}
}

void EventLoop::update(Channel* channel)
{
	poller_->update(channel);
}
