#pragma once

#include <array>
#include <string>

#include <iostream>

template <typename T, size_t S>
struct MergeSortTopDown {
	static const std::string name() { return "MergeSortTopDown"; }

	static void merge(std::array<T, S> &array, size_t leftStart, size_t leftEnd, size_t rightStart, size_t rightEnd)
	{
		// temp list size
		size_t leftLength = leftEnd - leftStart;
		size_t rightLength = rightEnd - rightStart;

		// Temp lists
		auto left = std::vector<T>(leftLength);
		auto right = std::vector<T>(rightLength);

		typename std::array<T,S>::iterator arrayIterator;
		typename std::vector<T>::iterator leftIterator;
		typename std::vector<T>::iterator rightIterator;

		// Copy into temp lists
		std::copy(array.begin() + leftStart, array.begin() + leftEnd, left.begin());
		std::copy(array.begin() + rightStart, array.begin() + rightEnd, right.begin());

		// Merge values into array
		for(arrayIterator = array.begin() + leftStart, leftIterator = left.begin(), 
			rightIterator = right.begin();
			leftIterator != left.end() && rightIterator != right.end();
			arrayIterator++)
		{
			auto& arrayElem = *arrayIterator;
			auto leftElem = *leftIterator;
			auto rightElem = *rightIterator;

			if(leftElem < rightElem)
			{
				arrayElem = leftElem;
				leftIterator++;
			}
			else
			{
				arrayElem = rightElem;
				rightIterator++;
			}
		}

		// Copy left-overs to array
		arrayIterator = std::copy(leftIterator, left.end(), arrayIterator);
		std::copy(rightIterator, right.end(), arrayIterator);
	}

	static void mergesort(std::array<T, S> &array, size_t left, size_t right)
	{
		if(right-left <= 1)
			return;

		const size_t leftStart = left;
		const size_t leftEnd = (left+right) / 2;
		const size_t rightStart = leftEnd;
		const size_t rightEnd = right;

		mergesort(array, leftStart, leftEnd);
		mergesort(array, rightStart, rightEnd);

		merge(array, leftStart, leftEnd, rightStart, rightEnd);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		mergesort(array, 0, S);
	}
};

template <typename T, size_t S>
struct MergeSortBottomUp {
	static const std::string name() { return "MergeSortBottomUp"; }

	static void merge(std::array<T, S> &array, std::array<T, S> &list, 
		size_t left, size_t right, size_t end)
	{
		auto i0 = left;
		auto i1 = right;

		for(auto j = left; j != end; j++)
		{
			if (i0 < right && (i1 >= end || array[i0] <= array[i1]))
			{
				list[j] = array[i0];
				i0 = i0 + 1;
			}
			else
			{
				list[j] = array[i1];
				i1 = i1 + 1;
			}
		}
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		auto list = std::array<T,S>(array);

		for(size_t width = 1; width < S; width = 2 * width)
		{
			for(size_t i = 0; i < S; i = i + 2 * width)
			{
				merge(array, list, i, std::min(i+width, S), std::min(i+2*width, S));
			}

			std::copy(list.begin(), list.end(), array.begin());
		}
	}
};
