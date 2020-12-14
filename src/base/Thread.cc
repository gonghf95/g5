#include "src/base/Thread.h"

#include <sys/syscall.h>

namespace base
{
namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
	CurrentThread::t_cachedTid = 0;
	CurrentThread::t_threadName = "main";
	CurrentThread::tid();
}

class ThreadNameInitializer
{
public:
	ThreadNameInitializer()
	{
		CurrentThread::t_threadName = "main";
		CurrentThread::tid();
		pthread_atfork(NULL, NULL, &afterfork);
	}
};

ThreadNameInitializer init;

struct ThreadData
{
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc func_;
	string name_;
	pid_t* tid_;

};

}
}

