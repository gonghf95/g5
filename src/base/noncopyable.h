#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

namespace base
{

class noncopyable
{
public:
	noncopyable(const noncopyable&) = delete;
	void operator=(const noncopyable&) = delete;

protected:
	noncopyable() = default;
	~noncopyable() = default;
};

} // namespace base

#endif
