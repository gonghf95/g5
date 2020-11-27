#ifndef BUFFER_H
#define BUFFER_H

#include <string>

namespace net
{

class Buffer
{
public:
	Buffer();
	virtual ~Buffer();

	const char* peek();
	int readableBytes() const;
	void retrieve(int len);
	void append(const std::string& data);
	std::string retrieveAsString(size_t len);
	std::string retrieveAllAsString();

private:
	std::string buf_;
};

} // namespace net

#endif
