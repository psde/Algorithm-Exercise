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
	const unsigned int ArraySize = 1000u;

	// Base class for all test fixtures
	template<class T, size_t Size>
	class ArrayTest : public testing::Test
	{
	protected:
		virtual void generateArray() = 0;
		std::unique_ptr<std::array<T, Size>> array;

		virtual void SetUp()
		{
			generateArray();
		}
	};

	// Testing class which generates a random array
	template<class T>
	class RandomArray : public ArrayTest<T, ArraySize>
	{
	protected:
		virtual void generateArray()
		{
			this->array = std::move(Tests::generateRandomArray<T, ArraySize>());
		}
	};

	// Testing class which generates an already sorted, ascending array
	template<class T>
	class AscendingArray : public ArrayTest<T, ArraySize>
	{
	protected:
		void generateArray()
		{
			this->array = std::move(Tests::generateAscendingArray<T, ArraySize>());
		}
	};

	// Testing class which generates a descending array
	template<class T>
	class DescendingArray : public ArrayTest<T, ArraySize>
	{
	protected:
		void generateArray()
		{
			this->array = std::move(Tests::generateDescendingArray<T, ArraySize>());
		}
	};

	template<class T>
	class EmptyArray : public ArrayTest<T, 0>
	{
	protected:
		void generateArray()
		{
			this->array.reset(new std::array<T, 0>());
		}
	};

	template<class T>
	class OneSizedArray : public ArrayTest<T, 1>
	{
	protected:
		void generateArray()
		{
			this->array = std::move(Tests::generateRandomArray<T, 1>());
		}
	};

	// All types that are used in testing
	using SortingTypes = ::testing::Types<int, float, double>;

	// Register types for the three test cases
	TYPED_TEST_CASE(RandomArray, Tests::SortingTypes);
	TYPED_TEST_CASE(AscendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(DescendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(EmptyArray, Tests::SortingTypes);
	TYPED_TEST_CASE(OneSizedArray, Tests::SortingTypes);
}

// Macro for testing different sorters
#define SORTER_TEST_CASE(CLASS) \
TYPED_TEST(RandomArray, CLASS) \
{ \
	for(unsigned int i = 0; i <= 25; i++) \
	{ \
		this->generateArray(); \
		CLASS<TypeParam, ArraySize>::sort(*this->array.get()); \
		EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
	} \
} \
TYPED_TEST(AscendingArray, CLASS) \
{ \
	EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
	CLASS<TypeParam, ArraySize>::sort(*this->array.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
} \
TYPED_TEST(DescendingArray, CLASS) \
{ \
	EXPECT_FALSE(Tests::isSorted(*this->array.get())); \
	CLASS<TypeParam, ArraySize>::sort(*this->array.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
} \
TYPED_TEST(EmptyArray, CLASS) \
{ \
	CLASS<TypeParam, 0>::sort(*this->array.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
} \
TYPED_TEST(OneSizedArray, CLASS) \
{ \
	CLASS<TypeParam, 1>::sort(*this->array.get()); \
	EXPECT_TRUE(Tests::isSorted(*this->array.get())); \
}
