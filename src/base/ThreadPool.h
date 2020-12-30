#ifndef BASE_THREADPOOL_H
#define BASE_THREADPOol_H

#include "src/base/noncopyable.h"
#include "src/base/Mutex.h"
#include "src/base/Condition.h"
#include "src/base/Thread.h"

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <deque>

namespace base
{

class ThreadPool : noncopyable
{
public:
	typedef std::function<void ()> Task;

	explicit ThreadPool(const std::string& nameArg = std::string("ThreadPool"));
	~ThreadPool();

	void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
	void setThreadInitCallback(const Task& cb)
	{ threadInitCallback_ = cb; }

	void start(int numThreads);
	void stop();

	const std::string& name() const
	{ return name_; }

	size_t queueSize() const;

	void run(Task f);

private:
	bool isFull() const;
	void runInThread();
	Task take();

	mutable MutexLock mutex_;
	Condition notEmpty_;
	Condition notFull_;
	std::string name_;
	Task threadInitCallback_;
	std::vector<std::unique_ptr<Thread>> threads_;
	std::deque<Task> queue_;
	size_t maxQueueSize_;
	bool running_;
};

} // namespace base

#endif
