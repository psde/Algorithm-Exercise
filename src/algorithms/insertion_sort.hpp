#pragma once

#include <array>

namespace insertion_sort {
	template <typename T, size_t SIZE>
	void sort(std::array<T, SIZE> &array)
	{
		size_t i, j;
		for (i = 0; i < SIZE; i++)
		{
			j = i;
			T t = array[j];
			while (j > 0 && array[j - 1] > t)
			{
				array[j] = array[j - 1];
				j--;
			}
			array[j] = t;
		}
	}
}