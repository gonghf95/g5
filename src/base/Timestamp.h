#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>

using std::string;

class Timestamp
{
public:
	Timestamp()
		: microSecondsSinceEpoch_(0)
	{
	}

	explicit Timestamp(int64_t ms)
		: microSecondsSinceEpoch_(ms)
	{
	}

	void swap(Timestamp& that)
	{
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}

	string toString() const;

	string toFormattedString(bool showMicroseconds = true) const;

	bool valid() const { return microSecondsSinceEpoch_ > 0; }

	int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
	time_t secondsSinceEpoch() const
	{
		return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	}

	static Timestamp now();
	static Timestamp invalid()
	{
		return Timestamp();
	}

	static Timestamp fromUnixTime(time_t t)
	{
		return fromUnixTime(t, 0);
	}

	static Timestamp fromUnixTime(time_t t, int microSeconds)
	{
		return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microSeconds);
	}

	static const int kMicroSecondsPerSecond = 1000 * 1000;

	friend bool operator<(Timestamp lhs, Timestamp rhs);
	friend bool operator==(Timestamp lhs, Timestamp rhs);

private:
	int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch_ < rhs.microSecondsSinceEpoch_;
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch_ == rhs.microSecondsSinceEpoch_;
}

inline Timestamp addTime(const Timestamp& ts, int interval)
{
	int64_t delta = static_cast<int64_t>(interval * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(ts.microSecondsSinceEpoch() + delta);
}

#endif
