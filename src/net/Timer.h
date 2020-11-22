#ifndef TIMER_H
#define TIMER_H

#include "src/net/Callbacks.h"

namespace net
{

class Timer
{
public:
	Timer(TimerCallback cb, Timestamp when, double interval)
		: callback_(std::move(cb)),
		expiration_(when),
		interval_(interval),
		repeat_(interval > 0.0),
		sequence_(when.microSecondsSinceEpoch()) // FIXME:
	{
	}

	void run() const
	{
		callback_();
	}

	Timestamp expiration() const { return expiration_; }
	int64_t sequence() const { return sequence_; }
	bool repeat() const { return repeat_; }

	void restart(Timestamp now);

private:
	const TimerCallback callback_;
	Timestamp expiration_;
	const double interval_;	
	const bool repeat_;
	const int64_t sequence_;	
};

} // namespace net

#endif
