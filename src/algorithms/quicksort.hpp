#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct QuickSort {
	static const std::string name() { return "QuickSort"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		size_t count = rightBound - leftBound;

		if (rightBound <= 1)
			return;

		size_t pivotIndex = leftBound + (count / 2);
		T pivotVal = array[pivotIndex];

		size_t left = leftBound;
		size_t right = rightBound;

		while (left <= right)
		{
			while (array[left] < pivotVal)
			{
				left++;
			}

			while (array[right] > pivotVal)
			{
				right--;
			}

			if (left <= right)
			{
				std::swap(array[left], array[right]);
				left++;
				right--;
			}
		}

		if (leftBound < right)
			sortInternal(array, leftBound, right);

		if (left < rightBound)
			sortInternal(array, left, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		sortInternal(array, 0, S - 1);
	}
};

