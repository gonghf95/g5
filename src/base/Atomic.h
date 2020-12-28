#ifndef BASE_ATOMIC_H
#define BASE_ATOMIC_H

#include "src/base/noncopyable.h"

#include <stdint.h>

namespace base
{

namespace detail
{

template<class T>
class AtomicInteger : noncopyable
{
public:
	AtomicInteger()
		: value_(0)
	{
	}
	
	T get()
	{
		return __atomic_load_n(&value_, __ATOMIC_SEQ_CST);
		//return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndAdd(T x)
	{
		return __sync_fetch_and_add(&value_, x);
		//return __sync_val_fetch_and_add(&value_, x);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}



private:
	volatile T value_;
};

} // namespace detail

typedef detail::AtomicInteger<int32_t> AtomicInt32;
typedef detail::AtomicInteger<int64_t> AtomicInt64;

} // namespace base

#endif
