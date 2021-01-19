#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "src/base/noncopyable.h"
#include "src/base/Thread.h"
#include "src/base/MutexLock.h"
#include "src/base/Condition.h"
#include <functional>
#include <memory>
#include <string>

namespace net
{

class EventLoop;

class EventLoopThread : noncopyable
{
public:
	typedef std::function<void (EventLoop*)> ThreadInitCallback;

	EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(), const std::string& name = std::string());
	~EventLoopThread();

	EventLoop* startLoop();

private:
	void threadFunc();

	EventLoop* loop_;
	bool exiting_;
	Thread thread_;
	Mutex mutex_;
	Condition cond_;
	ThreadInitCallback callback_;

};

} // namespace net

#endif // namespace net
