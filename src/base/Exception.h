#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <exception>
#include <string>

namespace base
{

class Exception : public std::exception
{
public:
	Exception(std::string what);
	~Exception() noexcept override = default;

	const char* what() const noexcept override
	{
		return message_.c_str();
	}

	const char* stackTrace() const noexcept
	{
		return stack_.c_str();
	}

private:
	std::string message_;
	std::string stack_;
};

} // namespace base

#endif
