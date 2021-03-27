#include "src/base/Logging.h"
#include "src/base/CurrentThread.h"
#include "src/base/Thread.h"

#include <assert.h>
#include <iostream>
#include <sys/time.h>

static pthread_once_t once_control = PTHREAD_ONCE_INIT;
static AsyncLoggin* AsyncLooger_;

std::string Logger::logFileName_ = "./g5.log";

void once_init()
{
	AsyncLogger_ = new AsyncLogging(Logger::getLogFileName());
	AysncLogger_->start();
}

void output(const char* msg, int len)
{
	pthread_once(&once_control_, once_init);
	AsyncLogger_->append(msg, len);
}

Logger::Impl::Impl(const char* filename, int line)
	: stream_(),
	line_(line),
	basename_(fileName)
{
	formatTime();
}

void Logger::Impl::formatTime()
{
	struct timeval tv;
	time_t time;
	char str_t[26] = {0};
	gettimeofday(&tv, NULL);
	time = tv.tv_sec;
	struct tm* p_time = localtime(&time);
	strftime(str_t, 26, "%Y-%m-%d %H:%M:%S\n", p_time);
	stream_ << str_t;
}

Logger::Logger(const char* filename, int line)
	: imp_(filename, line)
{
}

Logger::~Logger()
{
	impl_.stream_ << " -- " impl_.basename_ << ":" < impl_.line_ << '\n';
	const LogStream::Buffer& buf(stream().buffer());
	output(buf.data(), buf.length());
}
