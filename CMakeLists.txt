cmake_minimum_required(VERSION 2.8)

project(algorithm-exercise)

# Variables
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "1")
set(APPLICATION_VERSION_PATCH "0")
set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")
set(APPLICATION_NAME ${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Google Test
# Override google test options
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add gtest and enable it
add_subdirectory(thirdparty/gtest-1.7.0)
enable_testing()

# Include the gtest library
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Compiler flags
set(CPP_COMPILER_FLAGS "")
if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Detected GNUCXX")
    set(CPP_COMPILER_FLAGS "-Wall -Wextra -std=c++11 -pedantic -g")
elseif(MSVC)
    message(STATUS "Detected MSVC")
    #add_definitions("-D_CRT_SECURE_NO_WARNINGS")
    set(CPP_COMPILER_FLAGS "-D_CRT_SECURE_NO_WARNINGS")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message(STATUS "Detected Clang++")
    set(CPP_COMPILER_FLAGS "-Wall -Wextra -Wno-c++98-compat -std=c++11 -pedantic")
else()
    message(STATUS "Unknown Compiler")
endif()

# Add src subdirectory and configure compiler
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src" PROPERTY COMPILE_DEFINITIONS ${CPP_COMPILER_FLAGS})