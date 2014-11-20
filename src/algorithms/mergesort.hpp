#pragma once

#include <array>
#include <string>

#include <iostream>

template <typename T, size_t S>
struct MergeSort {
	static const std::string name() { return "MergeSort"; }

	static void merge(std::array<T, S> &array, size_t leftStart, size_t leftEnd, size_t rightStart, size_t rightEnd)
	{
		// temp list size
		size_t leftLength = leftEnd - leftStart;
		size_t rightLength = rightEnd - rightStart;

		// Temp lists
		auto left = std::vector<T>(leftLength);
		auto right = std::vector<T>(rightLength);

		auto arrayIterator = array.begin();
		auto leftIterator = left.begin();
		auto rightIterator = right.begin();

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

