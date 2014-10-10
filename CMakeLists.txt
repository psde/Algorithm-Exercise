cmake_minimum_required(VERSION 3.0)

project(algorithm-exercise)

option(with-tests "Build all tests." ON)

# Variables
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "1")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(APPLICATION_NAME ${PROJECT_NAME})

# Add subdirs
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

# If we need to run tests
if (with-tests)
	# Override google test options
	SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# This adds another subdirectory, which has 'project(gtest)'.
	add_subdirectory(thirdparty/gtest-1.7.0)

	enable_testing()

	# Include the gtest library.
	include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

	# Add tests
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()