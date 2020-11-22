set(net_SRCS
 TimerQueue.cc
 Timer.cc
 buffer.cc
 eventloop.cc
 epoller.cc
 tcpconnection.cc
 acceptor.cc
 channel.cc
 tcpserver.cc
)

add_library(g5_net ${net_SRCS})
target_link_libraries(g5_net g5_base pthread rt)

add_subdirectory(tests)
