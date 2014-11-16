#pragma once

#include <array>
#include <random>
#include <memory>
#include <functional>

#include "TestUtil.hpp"

#include "gtest/gtest.h"

namespace Tests
{
	// Size of all arrays used in testing
	const unsigned int ArraySize = 10u;

	// Testing class which generates a random array
	template<class T>
	class RandomArray : public testing::Test
	{
	private:
		virtual void generateArray()
		{
			arr = std::move(Tests::generateRandomArray<T, ArraySize>());
		}

	protected:
		std::unique_ptr<std::array<T, ArraySize>> arr;

		virtual void SetUp()
		{
			generateArray();
		}
	};

	// Testing class which generates an already sorted, ascending array
	template<class T>
	class AscendingArray : public RandomArray<T>
	{
	private:
		void generateArray()
		{
			this->arr = std::move(Tests::generateAscendingArray<T, ArraySize>());
		}
	};

	// Testing class which generates a descending array
	template<class T>
	class DescendingArray : public RandomArray<T>
	{
	private:
		void generateArray()
		{
			this->arr = std::move(Tests::generateDescendingArray<T, ArraySize>());
		}
	};

	// All types that are used in testing
	using SortingTypes = ::testing::Types<int, float, double>;

	// Register types for the three test cases
	TYPED_TEST_CASE(RandomArray, Tests::SortingTypes);
	TYPED_TEST_CASE(AscendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(DescendingArray, Tests::SortingTypes);



}

// Macro for testing different sorters
#define SORTER_TEST_CASE(CLASS) \
TYPED_TEST(RandomArray, CLASS) \
{ \
	CLASS<TypeParam, ArraySize>::sort(*this->arr.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->arr.get())); \
} \
TYPED_TEST(AscendingArray, CLASS) \
{ \
	EXPECT_TRUE(Tests::isSorted(*this->arr.get())); \
	CLASS<TypeParam, ArraySize>::sort(*this->arr.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->arr.get())); \
} \
TYPED_TEST(DescendingArray, CLASS) \
{ \
	EXPECT_FALSE(Tests::isSorted(*this->arr.get())); \
	CLASS<TypeParam, ArraySize>::sort(*this->arr.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->arr.get())); \
}