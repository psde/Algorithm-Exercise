#pragma once

#include <array>
#include <random>
#include <memory>
#include <functional>

#include "TestUtil.hpp"

#include "algorithms/algorithms.hpp"

#include "gtest/gtest.h"

namespace Tests
{
	// Size of all arrays used in testing
	const unsigned int ArraySize = 100u;

	// All types that are used in testing
	using SortingTypes = ::testing::Types<int, float, double>;

	// Register types for the three test cases
	TYPED_TEST_CASE(RandomArray, Tests::SortingTypes);
	TYPED_TEST_CASE(AscendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(DescendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(EmptyArray, Tests::SortingTypes);
	TYPED_TEST_CASE(OneSizedArray, Tests::SortingTypes);

	// Base class for all test fixtures
	template<class T, size_t S>
	class ArrayTest : public testing::Test
	{
	protected:
		virtual void generateArray() = 0;
		static const size_t Size = S;
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

		void test(std::function<void(std::array<T, RandomArray::Size>&)> f)
		{
			for(unsigned int i = 0; i <= 25; i++)
			{
				this->generateArray();
				f(*this->array.get());
				EXPECT_TRUE(Tests::isSorted(*this->array.get()));
			}
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

		void test(std::function<void(std::array<T, AscendingArray::Size>&)> f)
		{
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
			f(*this->array.get());
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
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

		void test(std::function<void(std::array<T, DescendingArray::Size>&)> f)
		{
			EXPECT_FALSE(Tests::isSorted(*this->array.get()));
			f(*this->array.get());
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
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

		void test(std::function<void(std::array<T, EmptyArray::Size>&)> f)
		{
			f(*this->array.get());
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
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

		void test(std::function<void(std::array<T, OneSizedArray::Size>&)> f)
		{
			f(*this->array.get());
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
		}
	};
}

// Macro for testing different sorters
#define SORTER_TEST_CASE(CLASS) \
TYPED_TEST(RandomArray, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(AscendingArray, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(DescendingArray, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(EmptyArray, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(OneSizedArray, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
}
