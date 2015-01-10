#include <iostream>
#include <chrono>
#include <ctime>
#include <random>

#include "gtest/gtest.h"

#include "algorithms/algorithms.hpp"
#include "tests/TestUtil.hpp"
#include "benchmarks/Benchmark.hpp"

template<typename T, size_t S>
void printArray(std::array<T,S> &array)
{
	for(auto &e : array)
	{
		std::cout << e << ", ";
	}
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	std::cout << "Testing:" << std::endl;
	::testing::InitGoogleTest(&argc, argv);
	int tests = RUN_ALL_TESTS();

	//std::cout << std::endl << "Benchmarking:" << std::endl;
	//auto b = Benchmarks::Benchmark();
	
	return tests;
}