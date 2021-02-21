#include "src/base/AsyncLogging.h"

AsyncLogging::AsyncLogging(const std::string basename, int flushInterval)
	: flushInterval_(flushInterval),
	running_(false),
	basename_(basename),
	thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging")
{
}

void AsyncLogging::append(const char* logline, int len)
{
	
}
