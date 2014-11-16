#pragma once

#include <array>
#include <string>

template <typename T, size_t S>
struct ShellSort {
	static const std::string name() { return "ShellSort"; }

	static void sort(std::array<T, S> &array)
	{
		static const int gaps[] = { 1705, 701, 301, 132, 57, 23, 10, 4, 1 };
		
		int gapIndex = 0;
		int i = 0;
		for (auto gap : gaps)
		{
			if (S > gap)
			{
				gapIndex = i;
				break;
			}
			i++;
		}
		
		bool sorted = false;
		while (true)
		{
			sorted = true;
			int st = 0;
			for (int x = gaps[gapIndex]; x < S; x += gaps[gapIndex])
			{
				if (array[st] > array[x])
				{
					std::swap(array[st], array[x]);
					sorted = false;
				}
				st = x;
			}

			if (++gapIndex >= 8) 
				gapIndex = 8;

			if (sorted && gapIndex == 8) 
				break;
		}
	}
};

