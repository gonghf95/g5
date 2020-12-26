#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include "src/base/noncopyable.h"
#include "src/base/CountDownLatch.h"
#include "src/base/CurrentThread.h"
#include "src/base/Atomic.h"

#include <string>
#include <unistd.h>
#include <pthread.h>
#include <functional>

using std::string;

namespace base
{

class Thread : public noncopyable
{
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(ThreadFunc, const string& name = string());
	~Thread();

	void start();
	int join();

	bool started() const { return started_; }
	pid_t tid() const { return tid_; }
	const string& name() const { return name_; }

	static int numCreated() { return numCreated_.get(); }

private:
	void setDefaultName();

	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	pid_t tid_;
	ThreadFunc func_;
	string name_;
	CountDownLatch latch_;

	static AtomicInt32 numCreated_;
};

} // namespace base

#endif /* BASE_THREAD_H */
