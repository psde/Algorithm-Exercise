#include <random>
#include <memory>

#include "gtest/gtest.h"

#include "insertion_sort.hpp"

template<class T, size_t S>
std::array<T, S> generateRandomArray()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> f_rand(0.f, 1000.f);

	std::array<T, S> arr;

	for (int i = 0; i < S; ++i)
	{
		arr[i] = static_cast<T>(f_rand(gen));
	}

	return arr;
}

template<class T, size_t S>
bool isSorted(std::array<T, S> arr)
{
	for (int i = 1; i < S; ++i)
	{
		if (arr[i] < arr[i - 1])
		{
			return false;
		}
	}
	return true;
}

TEST(insertion_sort, RandomArray)
{
	auto a = generateRandomArray<float, 10000>();

	insertion_sort::sort(a);

	EXPECT_TRUE(isSorted(a));
}

TEST(insertion_sort_guard, RandomArray)
{
	auto a = generateRandomArray<float, 10000>();

	insertion_sort_guard::sort(a);

	EXPECT_TRUE(isSorted(a));
}

TEST(insertion_sort_guard_transformed, RandomArray)
{
	auto a = generateRandomArray<float, 10000>();

	insertion_sort_guard_transformed::sort(a);

	EXPECT_TRUE(isSorted(a));
}