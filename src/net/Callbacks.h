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

typedef std::function<void (const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp receiveTime);

} // namespace net

#endif /* CALLBACKS_H */
