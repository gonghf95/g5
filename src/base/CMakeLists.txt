set(base_SRCS
 CountDownLatch.cc
 CurrentThread.cc
 Thread.cc
 Timestamp.cc
 TimeZone.cc
 Date.cc
)

add_library(g5_base ${base_SRCS})
target_link_libraries(g5_base pthread rt)

add_subdirectory(tests)
