#ifndef BASE_MUTEXLOCK_H
#define BASE_MUTEXLOCK_H

#include "src/base/noncopyable.h"
#include "src/base/CurrentThread.h"
#include <pthread.h>

namespace base
{

class MutexLock : noncopyable
{
public:
	MutexLock() : holder_(0)
	{
		pthread_mutex_init(&mutex_, NULL);
	}

	~Mutex()
	{
		assert(holer_ == 0);
		pthread_mutex_destory(&mutex_);
	}

	void lock()
	{
		pthread_mutex_lock(&mutex_);
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t* getPthreadMutex()
	{
		return &mutex_;
	}

private:
	friend class Condition;

	class UnassignGuard
	{
	public:
		UnassignGuard(Mutex& owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard(MutexLock& mutex)
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};

	void assignHolder()
	{
		holder_ = CurrentThread::tid();	
	}

	void unassignHolder()
	{
		holder_ = 0;
	}

	pthread_mutex_t mutex_;
	pid_t holder_;
};

class MutexLockGuard : noncopyable
{
public:
	MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}

private:
	MuteLock mutex_;
};

} // namespace base

#endif
