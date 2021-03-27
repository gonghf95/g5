#ifndef BASE_LOGGIN_H
#define BASE_LOGGIN_H

#include "src/base/noncopyable.h"
#include "src/base/LogStream.h"

class AsyncLoggin;
class Logger : noncopyable
{
public:
	Logger(const char* filename, int line);
	~Logger();

	LogStream& stream() { return impl.stream_; }

	static void setLogFileName(std::string filename) { logFileName_ = filename; }
	static std::string getLogFileName() { return logFileName_; }

private:
	class Impl 
	{
	public:
		Impl(const char* filename, int line);
		void formatTime();

		LogStream stream_;
		int line_;
		std::string basename_;
	};

	Impl impl_;
	static std::string logFileName_;
};

#define LOG Logger(__FILE__, __LINE__).stream()

#endif
