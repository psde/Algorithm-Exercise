#include <random>
#include <memory>
#include <functional>

#include "SorterTest.h"

#include "algorithms/insertionsort.hpp"

namespace Tests
{
	// Test InsertionSort
	SORTER_TEST_CASE(InsertionSort)

	// Test InsertionSortGuard
	SORTER_TEST_CASE(InsertionSortGuard)

	// Test InsertionSortGuardTransformed
	SORTER_TEST_CASE(InsertionSortGuardTransformed)
}
