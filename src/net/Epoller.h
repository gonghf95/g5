#ifndef EPOLLER_H
#define EPOLLER_H

#include <vector>
#include <sys/epoll.h>

namespace net
{

#define MAXEVENTS 1024

class Channel;
class Epoller
{
public:
	Epoller();
	virtual ~Epoller();

	void poll(std::vector<Channel*>& channels);
	void update(Channel* channel);

private:
	int epfd_;
	struct epoll_event events_[MAXEVENTS];
};

} // namespace net

#endif
