#include <random>
#include <memory>
#include <functional>

#include "SorterTest.h"

#include "algorithms/algorithms.hpp"

namespace Tests
{
	SORTER_TEST_CASE(InsertionSort)

	SORTER_TEST_CASE(InsertionSortGuard)

	SORTER_TEST_CASE(InsertionSortGuardTransformed)

	SORTER_TEST_CASE(QuickSort)
}
