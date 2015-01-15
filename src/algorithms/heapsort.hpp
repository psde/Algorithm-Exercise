#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct Heapsort {
	static const std::string name() { return "Heapsort"; }

	static void buildHeap(std::array<T, S> &array, size_t n, size_t v)
	{
		size_t w = (2 * v) + 1;
		while (w < n)
		{
			if (w + 1 < n)
			{
				if (array[w + 1] > array[w])
				{
					w++;
				}
			}

			if (array[v] >= array[w])
			{
				return;
			}

			std::swap(array[v], array[w]);
			v = w;
			w = (2 * v) + 1;
		}
	}
	
	static void sort(std::array<T, S> &array)
	{
		size_t n = S;

		// Build heap
		for (size_t v = (n / 2) - 1; v < n; v--){
			buildHeap(array, n, v);
		}

		// Sort heap
		while (n > 1)
		{
			n--;
			std::swap(array[0], array[n]);
			buildHeap(array, n, 0);
		}
	}
};

