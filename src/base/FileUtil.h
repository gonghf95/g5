#ifndef BASE_FILEUTIL_H
#define BASE_FILEUTIL_H

#include "src/base/noncopyable.h"

#include <string>

namespace base
{

namespace FileUtil
{

class ReadSmallFile : noncopyable
{
public:
	ReadSmallFile(std::string filename);
	~ReadSmallFile();

	template<typename String>
	int readToString(int maxSize,
					String* content,
					int64_t* fileSize,
					int64_t* modifyTime,
					int64_t* createTime);
	
	int readToBuffer(int* size);

	const char* buffer() const { return buf_; }

	static const int kBufferSize = 64*1024;

private:
	int fd_;
	int err_;
	char buf_[kBufferSize];
};

template<typename String>
int readFile(std::string filename,
			int maxSize,
			String* content,
			int64_t* fileSize = NULL,
			int64_t* modifyTime = NULL,
			int64_t* createTime = NULL)
{
	FileUtil::ReadSmallFile file(filename);
	return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

} // namespace FileUtil

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
