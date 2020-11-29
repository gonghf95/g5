#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "src/base/Timestamp.h"

#include <iostream>
#include <functional>
#include <memory>

namespace net
{

using std::cout;
using std::endl;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void ()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;

typedef std::function<void (const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp receiveTime);

} // namespace net

#endif /* CALLBACKS_H */
