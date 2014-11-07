#include <random>
#include <memory>

#include "gtest/gtest.h"

#include "insertion_sort.hpp"

// Size of all arrays used in testing
const unsigned int ArraySize = 10000u;

// All types that are used in testing
typedef ::testing::Types<int, float, double> SortingTypes;

// Returns 'true' if an array is sorted, false otherwise
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

// Testing class which generates a random array
template<class T>
class RandomArray : public testing::Test
{
private:
	virtual std::array<T, ArraySize> generateArray()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> f_rand(0.f, 10000.f);

		std::array<T, ArraySize> arr;

		for (int i = 0; i < ArraySize; ++i)
		{
			arr[i] = static_cast<T>(f_rand(gen));
		}

		return arr;
	}

protected:
	std::array<T, ArraySize> arr;

	virtual void SetUp()
	{
		arr = generateArray();
	}
};

// Testing class which generates an already sorted, ascending array
template<class T>
class AscendingArray : public RandomArray<T>
{
private:
	std::array<T, ArraySize> generateArray()
	{
		std::array<T, ArraySize> arr;

		for (int i = 0; i < ArraySize; ++i)
		{
			arr[i] = static_cast<T>(i);
		}

		return arr;
	}
};

// Testing class which generates a descending array
template<class T>
class DescendingArray : public RandomArray<T>
{
private:
	std::array<T, ArraySize> generateDescendingArray()
	{
		std::array<T, ArraySize> arr;

		for (int i = 0; i < ArraySize; ++i)
		{
			arr[i] = static_cast<T>(ArraySize - i);
		}

		return arr;
	}
};

// Register types for the three test cases
TYPED_TEST_CASE(RandomArray, SortingTypes);
TYPED_TEST_CASE(AscendingArray, SortingTypes);
TYPED_TEST_CASE(DescendingArray, SortingTypes);

// InsertionSort testing
TYPED_TEST(RandomArray, InsertionSort)
{
	InsertionSort::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}
TYPED_TEST(AscendingArray, InsertionSort)
{
	EXPECT_TRUE(isSorted(arr));
	InsertionSort::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}
TYPED_TEST(DescendingArray, InsertionSort)
{
	EXPECT_FALSE(isSorted(arr));
	InsertionSort::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}

// InsertionSortGuard
TYPED_TEST(RandomArray, InsertionSortGuard)
{
	InsertionSortGuard::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}
TYPED_TEST(AscendingArray, InsertionSortGuard)
{
	EXPECT_TRUE(isSorted(arr));
	InsertionSortGuard::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}
TYPED_TEST(DescendingArray, InsertionSortGuard)
{
	EXPECT_FALSE(isSorted(arr));
	InsertionSortGuard::sort(arr);
	EXPECT_TRUE(isSorted(arr));
}