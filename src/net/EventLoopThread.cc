#include "src/net/EventLoopThread.h"

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb, const std::string& name)
	: loop_(NULL),
	exiting_(false),
	mutex_(),
	thread_(std::bind(&EventLoopThread::threadFunc, this), name),
	condition_(mutex_),
	callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	if(loop_ != NULL)
	{
		loop_->quit();
		thread_.join();
	}
}

EventLoop* EventLoopThread::startLoop()
{
	assert(!thread_.started());
	thread_.start();
	
	EventLoop* loop = NULL;
	{
		MutexLockGuard lock(mutex_);
		while(loop_  == NULL)
		{
			cond_.wait();
		}
		loop = loop_;
	}
	return loop;
}

void EventLoop::threadFunc()
{
	EventLoop loop;

	if(callback_)
	{
		callback_(&loop);
	}

	{
		MutexLockGuard lock(mutex_);
		loop_ = &loop;
		cond_.notify();
	}

	loop.loop();
	
	MutexLockGuard lock(mutex_);
	loop_ = NULL;
}
