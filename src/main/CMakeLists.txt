# Source files
list(APPEND SRC_SOURCE
	main.cpp
)
SOURCE_GROUP("Source" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_SOURCE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../algorithms)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} algorithms)