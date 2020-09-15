#ifndef CHANNEL_H
#define CHANNEL_H

class IChannelCallback;
class Channel
{
public:
	Channel(int epollfd, int fd);
	~Channel();
	
	void setCallback(IChannelCallback* callback);
	void setRevents(int revent);
	void handleEvent();

	int fd() { return fd_; }
	int events() { return events_; }

	void enableReading();

private:
	int epollfd_;
	int fd_;
	int events_;
	int revents_;
	IChannelCallback* callback_;
};

#endif
