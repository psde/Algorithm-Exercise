# Test files
list(APPEND SRC_SOURCE
	SorterTestUtil.h
	SorterTest.cpp
)
SOURCE_GROUP("Tests" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_SOURCE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/algorithms)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${SOURCES})
target_link_libraries (${PROJECT_NAME}_test algorithms gtest gtest_main)