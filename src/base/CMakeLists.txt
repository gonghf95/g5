set(base_SRCS
 Logging.cc
 LogStream.cc
 AsyncLogging.cc
 LogFile.cc
 FileUtil.cc
 ThreadPool.cc
 Exception.cc
 Condition.cc
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
