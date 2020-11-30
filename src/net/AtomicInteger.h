#ifndef ATOMIC_INTEGER_H
#define ATOMIC_INTEGER_H

#include <stdint.h>

namespace net
{

namespace detail
{

template<class T>
class AtomicInteger
{
public:
	AtomicInteger()
		: value_(0)
	{
	}
	
	T get()
	{
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndAdd(T x)
	{
		return __sync_val_fetch_and_add(&value_, x);
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

} // namespace net

#endif
