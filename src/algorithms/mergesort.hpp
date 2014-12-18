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
		auto left = std::make_shared<std::vector<T>>(leftLength);
		auto right = std::make_shared<std::vector<T>>(rightLength);

		typename std::array<T, S>::iterator arrayIterator;
		typename std::vector<T>::iterator leftIterator;
		typename std::vector<T>::iterator rightIterator;

		// Copy into temp lists
		std::copy(array.begin() + leftStart, array.begin() + leftEnd, left->begin());
		std::copy(array.begin() + rightStart, array.begin() + rightEnd, right->begin());

		// Merge values into array
		for (arrayIterator = array.begin() + leftStart, leftIterator = left->begin(), rightIterator = right->begin();
			leftIterator != left->end() && rightIterator != right->end();
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
		arrayIterator = std::copy(leftIterator, left->end(), arrayIterator);
		std::copy(rightIterator, right->end(), arrayIterator);
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

	static void merge(std::array<T, S> &array, std::array<T, S> *tmp,
		size_t start, size_t middle, size_t end)
	{
		auto left = start;
		auto right = middle;

		for(auto j = left; j < end; j++)
		{
			if (left < middle && (right >= end || array[left] <= array[right]))
			{
				tmp->operator[](j) = array[left];
				left++;
			}
			else
			{
				tmp->operator[](j) = array[right];
				right++;
			}
		}
		
		std::copy(tmp->begin() + start, tmp->begin() + end, array.begin() + start);
	}

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		auto tmp = std::make_shared<std::array<T,S>>(array);

		for(size_t width = 1; width < S; width = 2 * width)
		{
			for(size_t i = 0; i < S; i += 2 * width)
			{
				merge(array, tmp.get(), i, std::min(i + width, S), std::min(i + 2 * width, S));
			}
		}
	}
};

template <typename T, size_t S>
struct MergeSortNatural {
	static const std::string name() { return "MergeSortNatural"; }

	static void sort(std::array<T, S> &array)
	{
		if(S <= 1)
			return;

		auto tmp = std::make_shared<std::array<T, S>>(array);

		size_t left = 0;
		size_t right = S - 1;
		bool sorted = false;
		size_t l = 0;
		size_t r = right;

		do {
			sorted = true;
			left = 0;

			while (left < right) 
			{
				l = left;
				while (l < right && array[l] <= array[l + 1]) 
				{
					l++;
				}

				r = l + 1;

				while (r == right - 1 || (r < right && array[r] <= array[r + 1]))
				{
					r++;
				}

				if (r <= right) 
				{
					if (r >= S)
						r = S - 1;
					MergeSortBottomUp<T,S>::merge(array, tmp.get(), left, l + 1, r + 1);
					sorted = false;
				}

				left = r + 1;
			}
		} while (!sorted);
	}
};