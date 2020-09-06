#ifndef G5_BASE_NONCOPYABLE_H
#define G5_BASE_NONCOPYABLE_H

namespace g5
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

} // namespace g5

#endif
