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
	using SortingTypes = ::testing::Types<int, float, double, std::string>;
	using SpecialTypes = ::testing::Types<int>;

	// Register types for the generic test cases
	TYPED_TEST_CASE(RandomArray, Tests::SortingTypes);
	TYPED_TEST_CASE(AscendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(DescendingArray, Tests::SortingTypes);
	TYPED_TEST_CASE(EmptyArray, Tests::SortingTypes);
	TYPED_TEST_CASE(OneSizedArray, Tests::SortingTypes);

	// Register types for specialized test cases
	TYPED_TEST_CASE(InsertionSortGuardMinimumFront, Tests::SpecialTypes);
	TYPED_TEST_CASE(InsertionSortGuardMinimumBack, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortOdd, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortAlwaysLeft, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortAlwaysRight, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortNaturalBitonic, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortNaturalBitonicInverted, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortNaturalBitonic2, Tests::SpecialTypes);
	TYPED_TEST_CASE(MergeSortNaturalBitonic2Inverted, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSortAllLeft, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSortAllRight, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSortAllEqual, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSort3WayAlmostEqual, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSortHybridInsertion, Tests::SpecialTypes);
	TYPED_TEST_CASE(QuickSortHybridMerge, Tests::SpecialTypes);
	
	// Base class for all test fixtures
	template<class T, size_t S>
	class ArrayTest : public testing::Test
	{
	protected:
		virtual void generateArray() = 0;
		static const size_t Size = S;
		std::unique_ptr<std::array<T, Size>> array;

		std::unique_ptr<std::array<T, S>> copyArray()
		{
			auto tmp = std::unique_ptr<std::array<T, S>>(new std::array<T, S>(*this->array.get()));
			return std::move(tmp);
		}

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
			this->array = std::move(TestArrays<T, ArraySize>::generateRandomArray());
		}

		void test(std::function<void(std::array<T, RandomArray::Size>&)> f)
		{
			for(unsigned int i = 0; i < 50; i++)
			{
				this->generateArray();

				auto tmp = ArrayTest<T, ArraySize>::copyArray();
				f(*tmp.get());
				bool sorted = Tests::isSorted(*tmp.get());
				EXPECT_TRUE(sorted);

				bool sameElements = Tests::hasSameElements(*this->array.get(), *tmp.get());
				EXPECT_TRUE(sameElements);

				// Fast exit if test failed
				if (sorted == false || sameElements == false)
					break;
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
			this->array = std::move(TestArrays<T, ArraySize>::generateAscendingArray());
		}

		void test(std::function<void(std::array<T, AscendingArray::Size>&)> f)
		{
			auto tmp = ArrayTest<T, ArraySize>::copyArray();
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	// Testing class which generates a descending array
	template<class T>
	class DescendingArray : public ArrayTest<T, ArraySize>
	{
	protected:
		void generateArray()
		{
			this->array = std::move(TestArrays<T, ArraySize>::generateDescendingArray());
		}

		void test(std::function<void(std::array<T, DescendingArray::Size>&)> f)
		{
			auto tmp = ArrayTest<T, ArraySize>::copyArray();
			EXPECT_FALSE(Tests::isSorted(*tmp.get()));
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	// Check if an empty array is handled
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

	// Check if an array with size one is handled
	template<class T>
	class OneSizedArray : public ArrayTest<T, 1>
	{
	protected:
		void generateArray()
		{
			this->array = std::move(TestArrays<T, 1>::generateRandomArray());
		}

		void test(std::function<void(std::array<T, OneSizedArray::Size>&)> f)
		{
			f(*this->array.get());
			EXPECT_TRUE(Tests::isSorted(*this->array.get()));
		}
	};

	template<class T>
	class InsertionSortGuardMinimumFront : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
				{ 1, 2, 3, 3, 2 }));
		}

		void test(std::function<void(std::array<T, InsertionSortGuardMinimumFront::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class InsertionSortGuardMinimumBack : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
				{ 2, 3, 3, 2, 1 }));
		}

		void test(std::function<void(std::array<T, InsertionSortGuardMinimumBack::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};	
	
	template<class T>
	class MergeSortOdd : public ArrayTest<T, 3>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 3>>(new std::array<T, 3>(
			{ 1, 2, 3 }));
		}

		void test(std::function<void(std::array<T, MergeSortOdd::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 3>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class MergeSortAlwaysLeft : public ArrayTest<T, 6>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 6>>(new std::array<T, 6>(
			{ 2, 2, 3, 3, 4, 4 }));
		}

		void test(std::function<void(std::array<T, MergeSortAlwaysLeft::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 6>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class MergeSortAlwaysRight : public ArrayTest<T, 6>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 6>>(new std::array<T, 6>(
			{ 4, 4, 3, 3, 2, 2 }));
		}

		void test(std::function<void(std::array<T, MergeSortAlwaysRight::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 6>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};
	
	template<class T>
	class MergeSortNaturalBitonic : public ArrayTest<T, 6>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 6>>(new std::array<T, 6>(
			{ 1, 2, 3, 3, 2, 1 }));
		}

		void test(std::function<void(std::array<T, MergeSortNaturalBitonic::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 6>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class MergeSortNaturalBitonicInverted : public ArrayTest<T, 6>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 6>>(new std::array<T, 6>(
			{ 3, 2, 1, 1, 2, 3 }));
		}

		void test(std::function<void(std::array<T, MergeSortNaturalBitonicInverted::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 6>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class MergeSortNaturalBitonic2 : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
			{ 1, 2, 3, 2, 1 }));
		}

		void test(std::function<void(std::array<T, MergeSortNaturalBitonic2::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class MergeSortNaturalBitonic2Inverted : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
			{ 3, 2, 1, 2, 3 }));
		}

		void test(std::function<void(std::array<T, MergeSortNaturalBitonic2Inverted::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class QuickSortAllLeft : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
			{ 3, 3, 4, 3, 3}));
		}

		void test(std::function<void(std::array<T, QuickSortAllLeft::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class QuickSortAllRight : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
			{ 3, 3, 2, 3, 3 }));
		}

		void test(std::function<void(std::array<T, QuickSortAllRight::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	template<class T>
	class QuickSortAllEqual : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
			{ 3, 3, 3, 3, 3 }));
		}

		void test(std::function<void(std::array<T, QuickSortAllEqual::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};
	
	template<class T>
	class QuickSort3WayAlmostEqual : public ArrayTest<T, 10>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 10>>(new std::array<T, 10>(
			{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 1 }));
		}

		void test(std::function<void(std::array<T, QuickSort3WayAlmostEqual::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 10>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	// Small array so that hybrid quicksort will use insertion sort
	template<class T>
	class QuickSortHybridInsertion : public ArrayTest<T, 5>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 5>>(new std::array<T, 5>(
				{ 1, 2, 3, 4, 5 }));
		}

		void test(std::function<void(std::array<T, QuickSortHybridInsertion::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 5>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
		}
	};

	// Unbalanced array so that hybrid quicksort will have to merge
	template<class T>
	class QuickSortHybridMerge : public ArrayTest<T, 15>
	{
	protected:
		void generateArray()
		{
			this->array = std::unique_ptr<std::array<T, 15>>(new std::array<T, 15>(
				{ 0, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }));
		}

		void test(std::function<void(std::array<T, QuickSortHybridMerge::Size>&)> f)
		{
			auto tmp = ArrayTest<T, 15>::copyArray();
			f(*tmp.get());
			EXPECT_TRUE(Tests::isSorted(*tmp.get()));
			EXPECT_TRUE(Tests::hasSameElements(*this->array.get(), *tmp.get()));
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
} \
TYPED_TEST(InsertionSortGuardMinimumFront, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(InsertionSortGuardMinimumBack, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortOdd, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortAlwaysLeft, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortAlwaysRight, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortNaturalBitonic, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortNaturalBitonicInverted, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortNaturalBitonic2, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(MergeSortNaturalBitonic2Inverted, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSortAllLeft, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSortAllRight, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSortAllEqual, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSort3WayAlmostEqual, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSortHybridInsertion, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
} \
TYPED_TEST(QuickSortHybridMerge, CLASS) \
{ \
	this->test(CLASS<TypeParam, this->Size>::sort); \
}
// Last Line is needed for macro!