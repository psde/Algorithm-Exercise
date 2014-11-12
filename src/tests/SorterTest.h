#pragma once

#include <array>
#include <random>
#include <memory>
#include <functional>

#include "TestUtil.h"

#include "gtest/gtest.h"

namespace SorterTest
{

	// Size of all arrays used in testing
	const unsigned int ArraySize = 1000u;

	// Testing class which generates a random array
	template<class T>
	class RandomArray : public testing::Test
	{
	private:
		virtual void generateArray()
		{
			arr = std::move(TestUtil::generateRandomArray<T, ArraySize>());
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
			this->arr = std::move(TestUtil::generateAscendingArray<T, ArraySize>());
		}
	};

	// Testing class which generates a descending array
	template<class T>
	class DescendingArray : public RandomArray<T>
	{
	private:
		void generateArray()
		{
			this->arr = std::move(TestUtil::generateDescendingArray<T, ArraySize>());
		}
	};

	// All types that are used in testing
	typedef ::testing::Types<int, float, double> SortingTypes;


	// Register types for the three test cases
	TYPED_TEST_CASE(RandomArray, SorterTest::SortingTypes);
	TYPED_TEST_CASE(AscendingArray, SorterTest::SortingTypes);
	TYPED_TEST_CASE(DescendingArray, SorterTest::SortingTypes);

	// Macro for testing different sorters
	#define SORTER_TEST_CASE(NAMESPACE) \
	TYPED_TEST(RandomArray, NAMESPACE) \
	{ \
		NAMESPACE<typename, ArraySize>::sort(*this->arr.get()); \
		EXPECT_TRUE(TestUtil::isSorted(*this->arr.get())); \
	} \
	TYPED_TEST(AscendingArray, NAMESPACE) \
	{ \
		EXPECT_TRUE(TestUtil::isSorted(*this->arr.get())); \
		NAMESPACE<typename, ArraySize>::sort(*this->arr.get()); \
		EXPECT_TRUE(TestUtil::isSorted(*this->arr.get())); \
	} \
	TYPED_TEST(DescendingArray, NAMESPACE) \
	{ \
		EXPECT_FALSE(TestUtil::isSorted(*this->arr.get())); \
		NAMESPACE<typename, ArraySize>::sort(*this->arr.get()); \
		EXPECT_TRUE(TestUtil::isSorted(*this->arr.get())); \
	}
}