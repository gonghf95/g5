#include "src/base/Exception.h"
#include "src/base/CurrentThread.h"

#include <memory>

namespace base
{

Exception::Exception(std::string msg) : message_(std::move(msg)),
	stack_(CurrentThread::stackTrace(false))
{
}

} // namespace base
