#include <random>
#include <memory>
#include <functional>

#include "SorterTest.hpp"

#include "algorithms/algorithms.hpp"

namespace Tests
{
	// InsertionSort
	SORTER_TEST_CASE(InsertionSort)
	SORTER_TEST_CASE(InsertionSortGuard)
	SORTER_TEST_CASE(InsertionSortGuardTransformed)

	// MergeSort
	SORTER_TEST_CASE(MergeSortTopDown)
	SORTER_TEST_CASE(MergeSortBottomUp)
	SORTER_TEST_CASE(MergeSortNatural)

	// QuickSort
	SORTER_TEST_CASE(QuickSort)
	SORTER_TEST_CASE(QuickSortShift)
	SORTER_TEST_CASE(QuickSortShift3Way)
	SORTER_TEST_CASE(QuickSort3WayHybrid)
	
	// Heapsort
	SORTER_TEST_CASE(Heapsort)
}
