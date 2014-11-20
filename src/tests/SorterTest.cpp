#include <random>
#include <memory>
#include <functional>

#include "SorterTest.hpp"

#include "algorithms/algorithms.hpp"

namespace Tests
{
	SORTER_TEST_CASE(InsertionSort)

	SORTER_TEST_CASE(InsertionSortGuard)

	SORTER_TEST_CASE(InsertionSortGuardTransformed)

	SORTER_TEST_CASE(MergeSort)

	SORTER_TEST_CASE(QuickSort)

	SORTER_TEST_CASE(ShellSort)
}
