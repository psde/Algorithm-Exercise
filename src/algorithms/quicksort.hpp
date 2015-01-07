#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct QuickSort {
	static const std::string name() { return "QuickSort"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) / 2;
		std::swap(array[pivotIndex], array[rightBound]);

		size_t i = leftBound - 1;
		size_t j = rightBound;
		T v = array[rightBound];
		
		for (;;)
		{
			while (array[++i] < v);
			while (v < array[--j])
			{
				if (j == leftBound)
					break;
			}
			if (i >= j)
				break;

			std::swap(array[i], array[j]);
		}
		std::swap(array[i], array[rightBound]);
		
		if (i != 0)
			sortInternal(array, leftBound, i - 1);

		sortInternal(array, i + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		sortInternal(array, 0, S - 1);
	}
};

template <typename T, size_t S>
struct QuickSortShift {
	static const std::string name() { return "QuickSortShift"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		size_t pivotIndex = (leftBound + rightBound) >> 1;
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
				if (left != right)
					std::swap(array[left], array[right]);

				left++;

				if (right > 0)
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

template <typename T, size_t S>
struct QuickSortShift3Way {
	static const std::string name() { return "QuickSortShift3Way"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		size_t pivotIndex = (leftBound + rightBound) >> 1;

		size_t left = leftBound;
		size_t right = rightBound;
		T pivotVal = array[pivotIndex];
		size_t i = leftBound;

		while (i <= right)
		{
			if (array[i] < pivotVal)
			{
				std::swap(array[left++], array[i++]);
			}
			else if (array[i] > pivotVal)
			{
				std::swap(array[i], array[right--]);
			}
			else
			{
				i++;
			}
		}

		if (leftBound < left)
			sortInternal(array, leftBound, left - 1);

		if (right < rightBound)
			sortInternal(array, right + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		sortInternal(array, 0, S - 1);
	}
};


template <typename T, size_t S>
struct QuickSort3WayHybrid {
	static const std::string name() { return "QuickSortHybrid"; }

	static void sortInsertion(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		size_t i, j;
		for (i = leftBound; i <= rightBound; i++){
			j = i;
			T t = array[j];
			while (j > leftBound && array[j - 1] > t){
				array[j] = array[j - 1];
				j--;
			}
			array[j] = t;
		}
	}

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		if (rightBound - leftBound < 9)
		{
			sortInsertion(array, leftBound, rightBound);
			return;
		}

		size_t pivotIndex = (leftBound + rightBound) >> 1;

		size_t left = leftBound;
		size_t right = rightBound;
		T pivotVal = array[pivotIndex];
		size_t i = leftBound;

		while (i <= right)
		{
			if (array[i] < pivotVal)
			{
				std::swap(array[left++], array[i++]);
			}
			else if (array[i] > pivotVal)
			{
				std::swap(array[i], array[right--]);
			}
			else
			{
				i++;
			}
		}

		if (leftBound < left)
			sortInternal(array, leftBound, left - 1);

		if (right + 1 < rightBound)
			sortInternal(array, right + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		sortInternal(array, 0, S - 1);
	}
};