#include "src/base/Thread.h"

#include <sys/syscall.h>

namespace base
{
namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{

}

}
}

