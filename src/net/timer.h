#ifndef TIMER_H
#define TIMER_H

#include "src/base/timestamp.h"
#include "src/net/iruncallback.h"

class Timer
{
public:
	Timer(Timestamp when, double interval, IRunCallback* cb)
		: expiration_(when), 
		 sequence_(when),
		 interval_(interval),
		 repeat_(interval > 0.0),
		 callback_(cb)
	{
	}

	void run() const
	{
		callback_->run();
	}

	Timestamp expiration() const { return expiration_; }
	Timestamp sequence() const { return sequence_; }
	bool repeat() const { return repeat_; }

	void restart(Timestamp now);

private:
	Timestamp expiration_;
	const Timestamp sequence_; // id
	const double interval_;
	const bool repeat_;
	IRunCallback* callback_;
};

#endif
