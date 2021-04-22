#include "src/base/FileUtil.h"
#include "src/base/types.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace base;

AppendFile::AppendFile(const std::string& filename)
	: fp_(fopen(filename.c_str(), "ae"))
{
	setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile()
{
	fclose(fp_);
}

void AppendFile::append(const char* logline, size_t len)
{
	size_t n = write(logline, len);
	size_t remain = len - n;
	while(remain > 0)
	{
		size_t x = write(logline + n, remain);
		if( x == 0 )
		{
			int err = ferror(fp_);
			if(err)
			{
				fprintf(stderr, "AppendFile::append() failed!\n");
			}
			break;
		}
		n += x;
		remain = len - n;
	}
}

void AppendFile::flush()
{
	fflush(fp_);
}

size_t AppendFile::write(const char* logline, size_t len)
{
	return fwrite_unlocked(logline, 1, len, fp_);
}
