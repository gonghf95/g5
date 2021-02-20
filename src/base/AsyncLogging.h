#ifndef BASE_ASYNCLOGGING_H
#define BASE_ASYNCLOGGING_H

#include "src/base/noncopyable.h"

namespace base
{

class AsyncLogging : noncopyable
{
public:
	AsyncLogging(const std::string& basename, int flushInterval = 2);
	~AsyncLogging()
	{
		if(running_)
		{
			stop();
		}
	}

	void start()
	{
		running_ = true;
		thread_.start();
		latch_.wait();
	}

	void stop()
	{
		running_ = false;
		cond_.notify();
		thread_.join();
	}

	void append(const char* logline, int len);

private:
	typedef FixedBuffer<kLargeBuffer> Buffer;
	typedef std::vector<std::shared_ptr<Buffer>> BufferVector;
	typedef std::shared_ptr<Buffer> BufferPtr;

	void threadFunc();

	const int flushInterval_;
	bool running_;
	std::string basename_;
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
	BufferPtr currentBuffer_;
	BufferPtr nextBuffer_;
	BufferPtr buffers_;
	CountDownLatch latch_;
};

} // namespace base

#endif
