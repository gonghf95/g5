#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include "src/base/noncopyable.h"
#include "src/base/Mutex.h"

namespace base
{

class Condition : noncopyable
{
public:
	Condition(MutexLock& mutex)
		: mutex_(mutex)
	{
		pthread_cond_init(&cond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&cond_);
	}

	void wait()
	{
		MutexLock::UnassignGuard ug(mutex_);
		pthread_cond_wait(&cond_, mutex_.getPthreadMutex());
	}

	void waitForSeconds(double seconds);

	void notify()
	{
		pthread_cond_signal(&cond_);
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&cond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t cond_;
};

} // namespace base

#endif
