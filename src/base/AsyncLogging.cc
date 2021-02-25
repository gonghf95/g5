#include "src/base/AsyncLogging.h"

AsyncLogging::AsyncLogging(const std::string basename, int flushInterval)
	: flushInterval_(flushInterval),
	running_(false),
	basename_(basename),
	thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
	mutex_(),
	cond_(mutex_),
	currentBuffer_(new Buffer),
	nextBuffer_(new Buffer),
	buffers_(),
	latch_(1)
{
	assert(basename_.size() > 1);
	currentBuffer_->bzero();
	nextBuffer_->bzero();
	buffers_.reserve(16);
}

void AsyncLogging::append(const char* logline, int len)
{
	MutexLockGuard lock(mutex_);
	if(currentBuffer_->avail() > len)
	{
		currentBuffer_->append(logline, len);
	}
	else
	{
		buffers_.push_back(currentBuffer_);
		currentBuffer_.reset();
		if(nextBuffer_)
		{
			currentBuffer_ = std::move(nextBuffer_);
		}
		else
		{
			currentBuffer_.reset(new Buffer);
		}
		currentBuffer_->append(logline, len);
		cond_.notify();
	}
}

void AsyncLogging::threadFunc()
{
	assert(running_ == true);
	LogFile output(basename_);
	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVector buffersToWrite;
	buffersToWrite.reserve(16);
	while(running_)
	{
		
	}
}
