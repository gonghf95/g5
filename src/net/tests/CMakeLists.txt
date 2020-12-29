add_executable(EventLoop_test EventLoop_test.cc)
target_link_libraries(EventLoop_test g5_base g5_net)

add_executable(TcpServer_test TcpServer_test.cc)
target_link_libraries(TcpServer_test g5_base g5_net)
