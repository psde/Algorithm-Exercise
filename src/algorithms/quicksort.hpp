#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct QuickSort {
	static const std::string name() { return "QuickSort"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		size_t count = rightBound - leftBound;

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
struct QuickSortShift {
	static const std::string name() { return "QuickSortShift"; }

	static void sortInternal(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		size_t count = rightBound - leftBound;

		size_t pivotIndex = leftBound + (count >> 1);
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

		size_t left = leftBound;
		size_t right = rightBound;
		T pivot = array[leftBound];
		size_t i = leftBound;

		while (i <= right)
		{
			if (array[i] < pivot)
			{
				std::swap(array[left++], array[i++]);
			}
			else if (array[i] > pivot)
			{
				std::swap(array[i], array[right--]);
			}
			else
			{
				i++;
			}
		}

		if (left != 0)
			sortInternal(array, leftBound, left - 1);

		sortInternal(array, right + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		sortInternal(array, 0, S - 1);
	}
};