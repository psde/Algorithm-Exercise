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
	auto array1 = TestUtil::generateRandomArray<double, 10000>().release();
	auto array2 = TestUtil::generateRandomArray<double, 20000>().release();
	auto array3 = TestUtil::generateRandomArray<double, 40000>().release();

	auto start = std::chrono::high_resolution_clock::now();

	auto foo = array1;
	InsertionSort::sort(*foo);

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = end - start;
	std::cout << duration.count() << std::endl;

	delete array1;
	delete array2;
	delete array3;
}

int main(int argc, char** argv)
{
	benchmark();
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}