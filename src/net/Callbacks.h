#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "src/base/timestamp.h"

#include <functional>
#include <memory>

namespace net
{

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void ()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;

} // namespace net

#endif /* CALLBACKS_H */