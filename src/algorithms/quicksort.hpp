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

			if (array[left] == v)
			{
				pLeft++;
				std::swap(array[pLeft], array[left]);
			}

			if (array[right] == v)
			{
				pRight--;
				std::swap(array[right], array[pRight]);
			}
		}
		std::swap(array[left], array[rightBound]);
		right = left - 1;
		left = left + 1;

		for (int k = leftBound; k < pLeft; k++, right--)
		{
			std::swap(array[k], array[right]);
		}

		for (int k = rightBound - 1; k > pRight; k--, left++)
		{
			std::swap(array[k], array[left]);			
		}

		if (left > 0)
			quickSort(array, left, rightBound);

		if (right > 0)
			quickSort(array, leftBound, right);
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
		int pRight = leftBound - 1;
		int pLeft = rightBound;
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

			if (array[left] == v)
			{
				pRight++;
				std::swap(array[pRight], array[left]);
			}

			if (array[right] == v)
			{
				pLeft--;
				std::swap(array[right], array[pLeft]);
			}
		}
		std::swap(array[left], array[rightBound]);
		right = left - 1;
		left = left + 1;

		for (int k = leftBound; k < pRight; k++, right--)
		{
			std::swap(array[k], array[right]);
		}

		for (int k = rightBound - 1; k > pLeft; k--, left++)
		{
			std::swap(array[k], array[left]);
		}

		if (left > 0 && right > 0)
		{
			size_t leftSize = leftBound - right;
			size_t rightSize = left - rightBound;

			if (leftSize >= rightSize * 5 || rightSize >= leftSize * 5)
			{
				QuickSortShift<T, S>::quickSort(array, leftBound, right );
				QuickSortShift<T, S>::quickSort(array, left, rightBound);

				auto tmp = std::make_shared<std::array<T, S>>(array);
				MergeSortBottomUp<T, S>::merge(array, tmp.get(), leftBound, (left + right) / 2, rightBound);
				return;
			}
		}

		if (left > 0)
			quickSort(array, left, rightBound);

		if (right > 0)
			quickSort(array, leftBound, right);
	}

	static void sort(std::array<T, S> &array)
	{
		if (S <= 1)
			return;

		quickSort(array, 0, S - 1);
	}
};