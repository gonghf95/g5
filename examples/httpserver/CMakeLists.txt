add_executable(httpserver httpserver.cc)
target_link_libraries(httpserver g5_base)
file(COPY index.html DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY favicon.ico DESTINATION ${EXECUTABLE_OUTPUT_PATH})
