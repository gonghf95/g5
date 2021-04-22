#ifndef BASE_FILEUTIL_H
#define BASE_FILEUTIL_H

#include "src/base/noncopyable.h"

#include <string>

namespace base
{

class AppendFile : noncopyable
{
public:
	explicit AppendFile(const std::string& filename);
	~AppendFile();

	void append(const char* logline, size_t len);

	void flush();

	off_t writtenBytes() const { return writtenBytes_; }

private:

	size_t write(const char* logline, size_t len);

	FILE* fp_;
	char buffer_[64*1024];
	off_t writtenBytes_;
};

} // namespace base

#endif
