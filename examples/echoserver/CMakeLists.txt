set(SRCS
 EchoServer.cc
 main.cc
)

add_executable(EchoServer ${SRCS})
target_link_libraries(EchoServer g5_base g5_net)
