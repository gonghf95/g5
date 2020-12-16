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
	CountDownLatch* latch_;

	ThreadData(ThreadFunc func, const string& name, pid* tid, CountDownLatch* latch) 
		: func_(std::move(func)),
		name_(name),
		tid_(tid),
		latch_(latch)
	{
	}

	void runInThread()
	{
		*tid_ = base::CurrentThread::tid();
		tid_ = NULL;
		latch_->countDown();
		latch_ = NULL;

		base::CurrentThread::t_threadName = name_.empty() ? "g5c_thread" : name_.c_str();
		::prctl(PR_SET_NAME, base::CurrentThread::t_threadName);
		try
		{
			func_();
			base::CurrentThread::t_threadName = "finished";
		}
		catch(const Exception& ex)
		{
			base::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", ex.what());
			fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
			abort();
		}
		catch(const std::exception& ex)
		{
			base::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			abort();
		}
		catch(...)
		{
			base::CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "unknow exception caught in Thread %s\n", name_.c_str());
			throw;
		}
	}
};

void* startThread(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runInThread();
	delete data;
	return NULL;
}

} // namespace detail


} // namespace base

