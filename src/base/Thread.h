#ifndef BASE_THREAD_H
#define BASE_THREAD_H

#include "src/base/noncopyable.h"
#include "src/net/AtomicInteger.h"

#include <string>
#include <unistd.h>
#include <pthread.h>

using std::string;

namespace base
{

class Thread : public noncopyable
{
public:
	typedef function<void()> ThreadFunc;

	explicit Thread(ThreadFunc, const string& name = string());
	~Thread();

	void start();
	int join();

	bool started() const { return started_; }
	pid_t tid() const { return tid_; }
	const string& name() const { return name_; }

	static int numCreated() { return numCreated_.get(); }

private:
	bool started_;
	bool joined_;
	pthread_t threadId_;
	pid_t tid_;
	ThreadFunc func_;
	string name_;
	CountDownLatch latch_;

	static net::AtomicInt32 numCreated_;
};

} // namespace base

#endif /* BASE_THREAD_H */
