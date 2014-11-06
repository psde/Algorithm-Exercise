# Source files
list(APPEND SRC_SOURCE
	insertion_sort.hpp
)
SOURCE_GROUP("Source" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_SOURCE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(algorithms ${SOURCES})