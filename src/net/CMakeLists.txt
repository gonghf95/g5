set(net_SRCS
 TimerQueue.cc
 Timer.cc
 Buffer.cc
 EventLoop.cc
 Epoller.cc
 TcpConnection.cc
 Acceptor.cc
 Channel.cc
 TcpServer.cc
)

add_library(g5_net ${net_SRCS})
target_link_libraries(g5_net g5_base pthread rt)

add_subdirectory(tests)
add_subdirectory(http)
