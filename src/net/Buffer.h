#ifndef BUFFER_H
#define BUFFER_H

#include "src/base/copyable.h"

#include <string>
#include <vector>

namespace net
{

class Buffer : public base::copyable
{
public:
	static const size_t kCheapPrepend = 8;
	static const size_t kInitialSize = 1024;

	explicit Buffer(size_t initialSize = kInitialSize)
		: buffer_(kCheapPrepend + initialSize),
		readerIndex_(kCheapPrepend),
		writerIndex_(kCheapPrepend)
	{
		assert(readableBytes() == 0);
		assert(writableBytes() == initialSize);
		assert(prependableBytes() == kCheapPrepend);
	}

	void swap(Buffer& rhs)
	{
		buffer_.swap(rhs.buffer_);
		std::swap(readerIndex_, rhs.readerIndex_);
		std::swap(writerIndex_, rhs.writerIndex_);
	}

	size_t readableBytes() const
	{ return writerIndex_ - readerIndex_; }

	size_t writableBytes() const
	{ return buffer_.size() - writerIndex_; }

	size_t prependableBytes() const
	{ return readerIndex_; }

	const char* peek()
	{ return begin() + readerIndex_; }

	const char* findCRLF() const
	{
		const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLf+2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findCRLF(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findEOL() const
	{
		const void* eol = memchr(peek(), '\n', readableBytes());
		return static_cast<const char*>(eol);
	}

	const char* findEOL(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		const void* eol = memchr(start, '\n', beginWrite() - start);
		return static_cast<const char*>(eol);
	}

	void retrieve(size_t len)
	{
		assert(len <= readableBytes());
		if(len < readableBytes())
		{
			readerIndex_ += len;
		}
		else
		{
			retrieveAll();
		}
	}

	void retrieveUntil(const char* end)
	{
		assert(peek() <= end);
		assert(end <= beginWrite());
		retrieve(end - peek());
	}

	void retrieveInt64()
	{
		retrieve(sizeof(int64_t));
	}

	void retrieveInt32()
	{
		retrieve(sizeof(int32_t));
	}

	void retrieveInt16()
	{
		retrieve(sizeof(int16_t));
	}

	void retrieveInt8()
	{
		retrieve(sizeof(int8_t));
	}

	void retrieveAll()
	{
		readerIndex_ = kCheapPrepend;
		writerIndex_ = kCheapPrepend;
	}

	std::string retrieveAllAsString()
	{
		return retrieveAsString(readableBytes());
	}

	std::string retrieveAsString(size_t len)
	{
		assert(len <= readableBytes());
		std::string result(peek(), len);
		retrieve(len);
		return result;
	}

	void append(const char* data, size_t len)
	{
		ensureWritableBytes(len);
		std::copy(data, data+len, beginWrite());
		writerIndex_ += len;
	}
	
	void append(const void* data, size_t len)
	{
		append(static_cast<const char*>(data), len);
	}

	void ensureWritableBytes(size_t len)
	{
		if(writableBytes() < len)
		{
			makeSpace(len);
		}
		assert(len <= writableBytes());
	}

	char* beginWrite()
	{
		return begin() + writerIndex_;
	}

	const char* beginWrite() const
	{
		return begin() + writerIndex_;
	}

	void hasWritten(size_t len)
	{
		assert(len <= writableBytes());
		writerIndex_ += len;
	}

	void unwrite(size_t len)
	{
		assert(len <= readableBytes());
		readerIndex_ -= len;
	}

	int64_t readInt64() const
	{
		int64_t x = peekInt64();
		retrieveInt64();
		return x;
	}

	int32_t readInt32() const
	{
		int32_t x = peekInt32();
		retrieveInt32();
		return x;
	}

	int16_t readInt16() const
	{
		int16_t x = peekInt16();
		retrieveInt16();
		return x;
	}

	int8_t readInt8() const
	{
		int8_t x = peekInt8();
		retrieveInt8();
		return x;
	}

	int64_t peekInt64() const
	{
		assert(readableBytes() >= sizeof(int64_t));
		int64_t x = 0;
		::memcpy(&x, peek(), sizeof(int64_t));
		return x;
	}

	int32_t peekInt32() const
	{
		assert(readableBytes() >= sizeof(int32_t));
		int32_t x = 0;
		::memcpy(&x, peek(), sizeof(int32_t));
		return x;
	}

	int16_t peekInt16() const
	{
		assert(readableBytes() >= sizeof(int16_t));
		int16_t x = 0;
		::memcpy(&x, peek(), sizeof(int16_t));
		return x;
	}

	int8_t peekInt8() const
	{
		assert(readableBytes() >= sizeof(int8_t));
		int8_t x = *peek();
		return x;
	}

private:
	char* begin()
	{ return &*buffer_.begin(); }
	
	const char* begin() const
	{ return &*buffer_.begin(); }

	void makeSpace(size_t len)
	{
	}

	std::vector<char> buffer_;
	size_t readerIndex_;
	size_t writerIndex_;

	static const char kCRLF[];
};

} // namespace net

#endif
