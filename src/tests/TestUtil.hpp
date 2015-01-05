#pragma once

#include <array>
#include <random>
#include <memory>
#include <functional>
#include <string>
#include <sstream>

namespace Tests
{
	// Returns 'true' if an array is sorted, false otherwise
	template<class T, size_t S>
	bool isSorted(std::array<T, S> arr)
	{
		for (size_t i = 1; i < S; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	template<typename T, size_t S>
	struct TestArrays
	{
		static std::unique_ptr<std::array<T, S>> generateRandomArray()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> f_rand(0, static_cast<float>(S)* 2.f);

			std::unique_ptr<std::array<T, S>> ary(new std::array<T, S>());

			for (size_t i = 0; i < S; ++i)
			{
				(*ary)[i] = static_cast<T>(static_cast<float>(S)-f_rand(gen));
			}

			return std::move(ary);
		}

		static std::unique_ptr<std::array<T, S>> generateAscendingArray()
		{
			std::unique_ptr<std::array<T, S>> ary(new std::array<T, S>());

			for (size_t i = 0; i < S; ++i)
			{
				(*ary)[i] = static_cast<T>(i);
			}

			return std::move(ary);
		}

		static std::unique_ptr<std::array<T, S>> generateDescendingArray()
		{
			std::unique_ptr<std::array<T, S>> ary(new std::array<T, S>());

			for (size_t i = 0; i < S; ++i)
			{
				(*ary)[i] = static_cast<T>(S - i);
			}

			return std::move(ary);
		}
	};

	template<size_t S>
	struct TestArrays<std::string, S>
	{
		static std::unique_ptr<std::array<std::string, S>> generateRandomArray()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> wordLengthRand(0, S);
			std::uniform_int_distribution<> charRand(26, 26 + 'a');

			std::unique_ptr<std::array<std::string, S>> ary(new std::array<std::string, S>());

			for (size_t i = 0; i < S; ++i)
			{
				size_t wordLength = wordLengthRand(gen);
				std::stringstream ss;

				for (size_t q = 0; q < wordLength; ++q)
				{
					ss << static_cast<char>(charRand(gen));
				}

				(*ary)[i] = ss.str();
			}

			return std::move(ary);
		}

		static std::unique_ptr<std::array<std::string, S>> generateAscendingArray()
		{
			std::unique_ptr<std::array<std::string, S>> ary(new std::array<std::string, S>());

			for (size_t i = 0; i < S; ++i)
			{
				size_t wordLength = i;
				std::string s;
				s.reserve(wordLength);

				for (size_t q = 0; q < wordLength; ++q)
				{
					s.append("a");
				}

				(*ary)[i] = s;
			}

			return std::move(ary);
		}

		static std::unique_ptr<std::array<std::string, S>> generateDescendingArray()
		{
			std::unique_ptr<std::array<std::string, S>> ary(new std::array<std::string, S>());

			for (size_t i = 0; i < S; ++i)
			{
				size_t wordLength = S - i;
				std::string s;
				s.reserve(wordLength);

				for (size_t q = 0; q < wordLength; ++q)
				{
					s.append("a");
				}

				(*ary)[i] = s;
			}

			return std::move(ary);
		}
	};
}