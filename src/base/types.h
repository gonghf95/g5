#ifndef G5_BASE_TYPES_H
#define G5_BASE_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>

namespace base
{

using std::string;

inline void memZero(void *p, size_t n)
{
	memset(p, 0, n);
}

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
	return f;
}

template<typename To, typename From>
inline To down_cast(From* f)
{
	return static_cast<To>(f);
}

} // namespace base

#endif
