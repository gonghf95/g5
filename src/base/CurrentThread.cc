#include "src/base/CurrentThread.h"

namespace base
{

namespace CurrentThread
{

__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char* t_threadName = "unknown";
static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

string statckTrace(bool demangle)
{

}

} // namespace CurrentThread

} // namespace base
