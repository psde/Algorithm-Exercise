# Algorithms files
list(APPEND SRC_ALGORITHMS
	algorithms/algorithms.hpp
	algorithms/insertionsort.hpp
	algorithms/quicksort.hpp
	algorithms/shellsort.hpp
	algorithms/mergesort.hpp
	algorithms/heapsort.hpp
)
SOURCE_GROUP("Algorithms" FILES ${SRC_ALGORITHMS})

# Tests sources
list(APPEND SRC_TESTS
	tests/SorterTest.cpp
	tests/SorterTest.hpp
	tests/TestUtil.hpp
)
SOURCE_GROUP("Tests" FILES ${SRC_TESTS})

# Benchmark sources
list(APPEND SRC_BENCHMARKS
	benchmarks/Benchmark.hpp
)
SOURCE_GROUP("Benchmarks" FILES ${SRC_BENCHMARKS})

# Main sources
list(APPEND SRC_MAIN
	main.cpp
)
SOURCE_GROUP("Main" FILES ${SRC_MAIN})

list(APPEND SOURCES ${SRC_ALGORITHMS} ${SRC_TESTS} ${SRC_BENCHMARKS} ${SRC_MAIN})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} gtest)