#include <iostream>
#include <chrono>
#include <ctime>
#include <random>

#include "gtest/gtest.h"
#include "tests/TestUtil.h"
#include "algorithms/insertion_sort.hpp"

template<size_t Size>
std::chrono::duration<double> benchmakSorter()
{
	return 0;
}

void benchmark()
{
	auto array = TestUtil::generateRandomArray<double, 40000>().release();

	auto start = std::chrono::high_resolution_clock::now();

	auto tmp = array;
	InsertionSort::sort(*tmp);

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = end - start;
	std::cout << duration.count() << std::endl;

	delete array;
}

int main(int argc, char** argv)
{
	benchmark();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}