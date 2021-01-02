#ifndef BASE_BLOCKINGQUEUE_H
#define BASE_BLOCKINGQUEUE_H

#include "src/base/Condition.h"
#include "src/base/Mutex.h"

#include <deque>
#include <assert.h>

namespace base
{

template<typename T>
class BlockingQueue : noncopyable
{
public:
	BlockingQueue() 
		: mutex_(),
		notEmpty_(mutex_),
		queue_()
	{
	}

	void put(const T& x)
	{
		MutexLockGuard lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify();
	}

	T take()
	{
		MutexLockGuard lock(mutex_);
		while(queue_.empty())
		{
			notEmpty_.wait();
		}

		assert(!queue_.empty());

		T front(std::move(queue_.front()));
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

private:
	mutable MutexLock mutex_;
	Condition notEmpty_;
	std::deque<T> queue_;
};

} // namespace base

#endif /* BASE_BLOCKINGQUEUE_H */
