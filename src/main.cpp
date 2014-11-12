#include <iostream>
#include <chrono>
#include <ctime>
#include <random>

#include "gtest/gtest.h"
#include "benchmarks/Benchmark.h"


int main(int argc, char** argv)
{
	auto b = benchmarks::Benchmark();
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}