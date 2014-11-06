#pragma once

#include <array>

namespace insertion_sort {
	template <typename T, size_t S>
	void sort(std::array<T, S> &array)
	{
		size_t i, j;
		for (i = 0; i < S; i++)
		{
			j = i;
			T t = array[j];
			while (j > 0 && array[j - 1] > t)
			{
				std::swap(array[j], array[j - 1]);
				j--;
			}
		}
	}
}

namespace insertion_sort_guard {
	template <typename T, size_t S>
	void sort(std::array<T, S> &array)
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
				std::swap(array[j], array[j - 1]);
				j--;
			}
		}
	}
}

namespace insertion_sort_guard_transformed {
	template <typename T, size_t S>
	void sort(std::array<T, S> &array)
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
				std::swap(array[j + 1], array[j]);
				j--;
			}
		}
	}
}