add_executable(http_server http_server.cc)
target_link_libraries(http_server g5_base)
file(COPY index.html DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY favicon.ico DESTINATION ${EXECUTABLE_OUTPUT_PATH})
