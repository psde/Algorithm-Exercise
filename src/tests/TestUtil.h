#pragma once

#include <array>
#include <random>
#include <memory>
#include <functional>

namespace TestUtil
{
	// Returns 'true' if an array is sorted, false otherwise
	template<class T, size_t S>
	bool isSorted(std::array<T, S> arr)
	{
		for (int i = 1; i < S; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, size_t S>
	std::unique_ptr<std::array<T, S>> generateRandomArray()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> f_rand(0.f, 10000.f);

		std::unique_ptr<std::array<T, S>> arr(new std::array<T, S>());

		for (int i = 0; i < S; ++i)
		{
			arr->operator[](i) = static_cast<T>(f_rand(gen));
		}

		return std::move(arr);
	}

	template<typename T, size_t S>
	std::unique_ptr<std::array<T, S>> generateAscendingArray()
	{
		std::unique_ptr<std::array<T, S>> arr(new std::array<T, S>());

		for (int i = 0; i < S; ++i)
		{
			arr->operator[](i) = static_cast<T>(i);
		}

		return std::move(arr);
	}

	template<typename T, size_t S>
	std::unique_ptr<std::array<T, S>> generateDescendingArray()
	{
		std::unique_ptr<std::array<T, S>> arr(new std::array<T, S>());

		for (int i = 0; i < S; ++i)
		{
			arr->operator[](i) = static_cast<T>(S - i);
		}

		return std::move(arr);
	}
}