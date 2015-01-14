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

		size_t left = leftBound;
		size_t right = rightBound;
		T v = array[rightBound];

		for (;;)
		{
			while (array[left] < v) {
				left++;
			}

			while (v < array[--right])
			{
				if (right == leftBound)
					break;
			}
			if (left >= right)
				break;

			std::swap(array[left], array[right]);
		}
		std::swap(array[left], array[rightBound]);

		if (left != 0)
			quickSort(array, leftBound, left - 1);

		quickSort(array, left + 1, rightBound);
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

		size_t left = leftBound;
		size_t right = rightBound;
		T v = array[rightBound];

		for (;;)
		{
			while (array[left] < v) {
				left++;
			}

			while (v < array[--right])
			{
				if (right == leftBound)
					break;
			}
			if (left >= right)
				break;

			std::swap(array[left], array[right]);
		}
		std::swap(array[left], array[rightBound]);

		if (left != 0)
			quickSort(array, leftBound, left - 1);

		quickSort(array, left + 1, rightBound);
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

	static void quickSort(std::array<T, S> &array, size_t leftBound, size_t rightBound)
	{
		if (rightBound <= leftBound)
			return;

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) >> 1;
		std::swap(array[pivotIndex], array[rightBound]);

		int left = leftBound - 1;
		int right = rightBound;
		int pLeft = leftBound - 1;
		int pRight = rightBound;
		T v = array[rightBound];

		for (;;)
		{
			while (array[++left] < v);

			while (v < array[--right])
			{
				if (right == leftBound)
					break;
			}
			if (left >= right)
				break;

			std::swap(array[left], array[right]);

			// Equal to left end
			if (array[left] == v)
			{
				pLeft++;
				std::swap(array[pLeft], array[left]);
			}

			// Equal to right end
			if (array[right] == v)
			{
				pRight--;
				std::swap(array[right], array[pRight]);
			}
		}
		std::swap(array[left], array[rightBound]);

		if (right > 0)
		{
			right = left - 1;

			if (pLeft < S)
			{
				// Equal partitions from left to middle
				for (int k = leftBound; k < pLeft; k++, right--)
				{
					std::swap(array[k], array[right]);
				}
			}

			quickSort(array, leftBound, right);
		}

		if (left < S - 1)
		{
			left = left + 1;
			
			// Equal partitions from right to middle
			for (int k = rightBound - 1; k > pRight; k--, left++)
			{
				std::swap(array[k], array[left]);
			}

			quickSort(array, left, rightBound);
		}
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

		// Swap pivot element to the end
		size_t pivotIndex = (leftBound + rightBound) >> 1;
		std::swap(array[pivotIndex], array[rightBound]);

		int left = leftBound - 1;
		int right = rightBound;
		int pLeft = leftBound - 1;
		int pRight = rightBound;
		T v = array[rightBound];

		for (;;)
		{
			while (array[++left] < v);

			while (v < array[--right])
			{
				if (right == leftBound)
					break;
			}
			if (left >= right)
				break;

			std::swap(array[left], array[right]);

			// Equal to left end
			if (array[left] == v)
			{
				pLeft++;
				std::swap(array[pLeft], array[left]);
			}

			// Equal to right end
			if (array[right] == v)
			{
				pRight--;
				std::swap(array[right], array[pRight]);
			}
		}
		std::swap(array[left], array[rightBound]);

		bool sortRight = false;
		bool sortLeft = false;

		if (right > 0)
		{
			right = left - 1;

			if (pLeft < S)
			{
				// Equal partitions from left to middle
				for (int k = leftBound; k < pLeft; k++, right--)
				{
					std::swap(array[k], array[right]);
				}
			}

			sortLeft = true;
		}

		if (left < S - 1)
		{
			left = left + 1;
			// Equal partitions from right to middle
			for (int k = rightBound - 1; k > pRight; k--, left++)
			{
				std::swap(array[k], array[left]);
			}

			sortRight = true;
		}

		if (sortRight && sortLeft)
		{
			size_t leftSize = leftBound - right;
			size_t rightSize = left - rightBound;

			if (leftSize >= rightSize * 5 || rightSize >= leftSize * 5)
			{
				size_t middle = (leftBound + rightBound) >> 1;
				QuickSortShift<T, S>::quickSort(array, leftBound, middle);
				QuickSortShift<T, S>::quickSort(array, middle, rightBound);

				auto tmp = std::make_shared<std::array<T, S>>(array);
				MergeSortBottomUp<T, S>::merge(array, tmp.get(), leftBound, middle, rightBound);
				return;
			}
		}

		if (sortLeft)
			quickSort(array, leftBound, right);

		if (sortRight)
			quickSort(array, left, rightBound);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};