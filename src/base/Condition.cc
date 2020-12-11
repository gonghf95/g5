#include "src/base/Condition.h"

#include <errno.h>

bool base::Condition::waitForSeconds(double seconds)
{
	struct timespec abstime;

	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNonoSecondsPerSecond = 1000000000;
	int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec += static_cast<time_t>((abstime.tv_nsec + nanseconds) % kNanoSecondsPerSecond);

	MutexLock::UnassignGuard ug(mutex_);
	return ETIMEOUT == pthread_cond_timedwait(&cond_, mutex_.getPthreadMutex(), &abstime);
}
