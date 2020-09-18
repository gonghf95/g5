set(SRCS
 echoserver.cc
 main.cc
)

add_executable(echoserver ${SRCS})
target_link_libraries(echoserver g5_base g5_net)
