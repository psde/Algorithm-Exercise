#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct InsertionSort {
	static const std::string name() { return "InsertionSort"; }
	static void sort(std::array<T, S> &array)
	{
		size_t i, j;
		for (i = 0; i < S; i++){
			j = i;
			T t = array[j];
			while (j > 0 && array[j - 1] > t){
				array[j] = array[j - 1];
				j--;
			}
			array[j] = t;
		}
	}
};

template <typename T, size_t S>
struct InsertionSortGuard {
	static const std::string name() { return "InsertionSortGuard"; }
	static void sort(std::array<T, S> &array)
	{
		size_t min = 0;
		for (size_t i = 0; i < S; i++)
		{
			if (array[i] < array[min])
				min = i;
		}

		if (min != 0)
			std::swap(array[0], array[min]);

		size_t i, j;
		for (i = 1; i < S; i++)
		{
			j = i;
			T t = array[j];
			while (array[j - 1] > t)
			{
				array[j] = array[j - 1];
				j--;
			}
			array[j] = t;
		}
	}
};

template <typename T, size_t S>
struct InsertionSortGuardTransformed {
	static const std::string name() { return "InsertionSortGuardTransformed"; }
	static void sort(std::array<T, S> &array)
	{
		size_t min = 0;
		for (size_t i = 0; i < S; i++)
		{
			if (array[i] < array[min])
				min = i;
		}

		if (min != 0)
			std::swap(array[0], array[min]);

		size_t i, j;
		for (i = 1; i < S; i++)
		{
			T t = array[i];
			j = i - 1;
			while (array[j] > t)
			{
				array[j + 1] = array[j];
				j--;
			}
			array[j + 1] = t;
		}

	}
};