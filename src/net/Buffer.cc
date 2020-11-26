#include "Buffer.h"

using std::string;

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

const char* Buffer::peek()
{
	return buf_.c_str();
}

int Buffer::readableBytes() const
{
	return buf_.size();
}

void Buffer::retrieve(int len)
{
	buf_ = buf_.substr(len, buf_.length() - len);
}

void Buffer::append(const string& data)
{
	buf_.append(data);
}

string Buffer::retrieveAsString(size_t len)
{
	string ret(peek(), len);
	retrieve(len);
	return ret;
}

string Buffer::retrieveAllAsString()
{
	return retrieveAsString(readableBytes());
}
