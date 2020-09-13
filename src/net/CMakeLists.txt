set(net_SRCS
 acceptor.cc
 channel.cc
 tcpserver.cc
)

add_library(g5_net ${net_SRCS})
target_link_libraries(g5_net pthread rt)

#add_subdirectory(tests)
