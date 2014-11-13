#include <iostream>
#include <chrono>
#include <ctime>
#include <random>

#include "gtest/gtest.h"
#include "benchmarks/Benchmark.h"


int main(int argc, char** argv)
{
	std::cout << "Testing:" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	int tests = RUN_ALL_TESTS();

	std::cout << std::endl << "Benchmarking:" << std::endl;
	auto b = benchmarks::Benchmark();
	
	return tests;
}