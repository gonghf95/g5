#ifndef BASE_COUNTDOWNLATCH_H
#define BASE_COUNTDOWNLATCH_H

#include "src/base/Mutex.h"
#include "src/base/Condition.h"

namespace base
{

class CountDownLatch : noncopyable
{
public:
	explicit CountDownLatch(int count);

	void wait();

	void countDown();

	int getCount() const;

private:
	mutable MutexLock mutex_;
	Condition condition_;
	int count_;
};

} // namespace base

#endif
