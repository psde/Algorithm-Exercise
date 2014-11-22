#pragma once

#include <array>
#include <string>

#include <iostream>

template <typename T, size_t S>
struct MergeSortTopDown {
	static const std::string name() { return "MergeSortTopDown"; }

	static void merge(std::array<T, S> &array, size_t leftStart, size_t rightStart, size_t rightEnd)
	{
		size_t leftEnd = rightStart;

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
		for(arrayIterator = array.begin() + leftStart, leftIterator = left.begin(), rightIterator = right.begin();
			leftIterator != left.end() && rightIterator != right.end();
			arrayIterator++)
		{
			// Maybe use std::swap?
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
		const size_t rightStart = (left+right) / 2;
		const size_t rightEnd = right;

		mergesort(array, leftStart, rightStart);
		mergesort(array, rightStart, rightEnd);

		merge(array, leftStart, rightStart, rightEnd);
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

	static void merge(std::array<T, S> &array, std::array<T, S> &tmp,
		size_t leftStart, size_t rightStart, size_t end)
	{
		auto left = leftStart;
		auto right = rightStart;

		for(auto j = leftStart; j != end; j++)
		{
			if (left < rightStart && (right >= end || array[left] <= array[right]))
			{
				tmp[j] = array[left];
				left = left + 1;
			}
			else
			{
				tmp[j] = array[right];
				right = right + 1;
			}
		}
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		auto tmp = std::array<T,S>(array);

		for(size_t width = 1; width < S; width = 2 * width)
		{
			for(size_t i = 0; i < S; i = i + 2 * width)
			{
				merge(array, tmp, i, std::min(i+width, S), std::min(i+2*width, S));
			}

			std::copy(tmp.begin(), tmp.end(), array.begin());
		}
	}
};