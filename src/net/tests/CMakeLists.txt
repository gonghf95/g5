add_executable(eventloop_test eventloop_test.cc)
target_link_libraries(eventloop_test g5_base g5_net)

add_executable(tcpserver_test tcpserver_test.cc)
target_link_libraries(tcpserver_test g5_base g5_net)
