#ifndef NET_EVENTLOOPTHREAD_H
#define NET_EVENTLOOPTHREAD_H

#include "src/base/noncopyable.h"
#include "src/base/Thread.h"
#include "src/base/Mutex.h"
#include "src/base/Condition.h"
#include <functional>
#include <memory>
#include <string>

using base::Thread;
using base::MutexLock;
using base::MutexLockGuard;
using base::Condition;

namespace net
{

class EventLoop;

class EventLoopThread : base::noncopyable
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
	MutexLock mutex_;
	Condition cond_;
	ThreadInitCallback callback_;

};

} // namespace net

#endif // namespace net
