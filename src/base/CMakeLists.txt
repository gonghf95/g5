set(base_SRCS
 task.cc
 threadpool.cc
 sysutils.cc
)

add_library(g5_base ${base_SRCS})
target_link_libraries(g5_base pthread rt)
