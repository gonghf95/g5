set(base_SRCS
 Timestamp.cc
 timezone.cc
 date.cc
)

add_library(g5_base ${base_SRCS})
target_link_libraries(g5_base pthread rt)

add_subdirectory(tests)
