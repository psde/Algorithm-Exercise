#pragma once

#include <array>
#include <string>
#include <memory>

#include "algorithms/mergesort.hpp"

template <typename T, size_t S>
struct QuickSort {
	static const std::string name() { return "QuickSort"; }

	static void quickSort(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) / 2;
		std::swap(array[pivotIndex], array[rightBound]);

		size_t i = leftBound;
		size_t j = rightBound;
		T v = array[rightBound];
		
		for (;;)
		{
			while (array[i] < v) {
				i++;
			}

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
			quickSort(array, leftBound, i - 1);

		quickSort(array, i + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};

template <typename T, size_t S>
struct QuickSortShift {
	static const std::string name() { return "QuickSortShift"; }

	static void quickSort(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) >> 1;
		std::swap(array[pivotIndex], array[rightBound]);

		size_t i = leftBound;
		size_t j = rightBound;
		T v = array[rightBound];

		for (;;)
		{
			while (array[i] < v) {
				i++;
			}

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
			quickSort(array, leftBound, i - 1);

		quickSort(array, i + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};

template <typename T, size_t S>
struct QuickSortShift3Way {
	static const std::string name() { return "QuickSortShift3Way"; }

	static void quickSort(std::array<T, S> &array, int leftBound, int rightBound)
	{
		if (rightBound <= leftBound)
			return;

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) >> 1;
		std::swap(array[pivotIndex], array[rightBound]);

		int i = leftBound - 1;
		int j = rightBound;
		int p = leftBound - 1;
		int q = rightBound;
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

			if (array[i] == v)
			{
				p++;
				std::swap(array[p], array[i]);
			}

			if (array[j] == v)
			{
				q--;
				std::swap(array[j], array[q]);
			}
		}
		std::swap(array[i], array[rightBound]);
		j = i - 1;
		i = i + 1;

		for (int k = leftBound; k < p; k++, j--)
		{
			std::swap(array[k], array[j]);

		}

		for (int k = rightBound - 1; k > q; k--, i++)
		{
			std::swap(array[k], array[i]);			

		}

		if (i > 0)
			quickSort(array, i, rightBound);

		if (j > 0)
			quickSort(array, leftBound, j);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};

template <typename T, size_t S>
struct QuickSort3WayHybrid {
	static const std::string name() { return "QuickSort3WayHybrid"; }

	static void insertionSort(std::array<T, S> &array, size_t leftBound, size_t rightBound)
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

	static void merge(std::array<T, S> &array, std::array<T, S> *tmp,
		size_t start, size_t middle, size_t end)
	{
		auto left = start;
		auto right = middle;

		for (auto j = left; j < end; j++)
		{
			if (left < middle && (right >= end || array[left] <= array[right]))
			{
				(*tmp)[j] = array[left];
				left++;
			}
			else
			{
				(*tmp)[j] = array[right];
				right++;
			}
		}

		std::copy(tmp->begin() + start, tmp->begin() + end, array.begin() + start);
	}

	static void quickSort(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		if (rightBound - leftBound < 9)
		{
			insertionSort(array, leftBound, rightBound);
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

		if (leftBound < left && right + 1 < rightBound)
		{
			size_t leftSize = leftBound - left;
			size_t rightSize = right + 1 - rightBound;


			if (leftSize >= rightSize * 1.5 || rightSize >= leftSize * 1.5)
			{
				QuickSortShift<T, S>::quickSort(array, leftBound, left - 1);
				QuickSortShift<T, S>::quickSort(array, right + 1, rightBound);

				auto tmp = std::make_shared<std::array<T, S>>(array);
				MergeSortBottomUp<T, S>::merge(array, tmp.get(), leftBound, (left + right) / 2, rightBound);
				return;
			}
		}

		if (leftBound < left)
			quickSort(array, leftBound, left - 1);

		if (right + 1 < rightBound)
			quickSort(array, right + 1, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};