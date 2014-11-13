#pragma once

#include <chrono>
#include <unordered_map>
#include <string>

#include "tests/TestUtil.h"
#include "algorithms/insertion_sort.hpp"

namespace benchmarks
{
	class Benchmark
	{
	private:

		std::vector<size_t> _sizes;

		using BenchmarkMap = std::unordered_map<std::string, std::vector<std::chrono::milliseconds>>;
		BenchmarkMap _random;
		BenchmarkMap _ascending;
		BenchmarkMap _descending;

		template<size_t X, size_t Y>
		struct pow_struct {
			static const size_t value = X * pow_struct<X, Y - 1>::value;
		};

		template<size_t X>
		struct pow_struct<X, 0> {
			static const size_t value = 1;
		};

		template<template <typename, size_t> class F, size_t S>
		std::chrono::milliseconds benchmarkSorterWithArray(std::array<double, S> *ary)
		{
			// Create a copy
			auto tmp = new std::array<double, S>(*ary);

			auto start = std::chrono::steady_clock::now();
			F<double, S>::sort(*tmp);
			auto end = std::chrono::steady_clock::now();

			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			delete tmp;

			return milliseconds;
		}

		template<template <typename, size_t> class F, size_t S>
		void benchmarkSorter()
		{
			auto random = TestUtil::generateRandomArray<double, S>().release();
			auto ascending = TestUtil::generateAscendingArray<double, S>().release();
			auto descending = TestUtil::generateDescendingArray<double, S>().release();

			_random[F<double, S>::name()].push_back(benchmarkSorterWithArray<F, S>(random));
			_ascending[F<double, S>::name()].push_back(benchmarkSorterWithArray<F, S>(ascending));
			_descending[F<double, S>::name()].push_back(benchmarkSorterWithArray<F, S>(descending));

			delete random;
			delete ascending;
			delete descending;

			std::cout << ".";
		}

		template<size_t I>
		void benchmarkIteration() {
			// Recursion first, so we get ascending sizes
			benchmarkIteration<I - 1>();
			
			const size_t ArraySize = 10000 * pow_struct<2, I - 1>::value;
			std::cout << "Benchmarking for size = " << ArraySize;
			_sizes.push_back(ArraySize);

			benchmarkSorter<InsertionSort, ArraySize>();
			benchmarkSorter<InsertionSortGuard, ArraySize>();
			benchmarkSorter<InsertionSortGuardTransformed, ArraySize>();

			std::cout << " done." << std::endl;
		};		

		template<>
		void benchmarkIteration<0>()
		{ }

		template<size_t I>
		void benchmark() {
			benchmarkIteration<I>();
		}

		void printBenchmarkMap(BenchmarkMap &map)
		{
			for (const auto &kv : map)
			{
				std::cout << kv.first << ",";

				for (const auto &ms : kv.second)
				{
					std::cout << ms.count() << ",";
				}
				std::cout << std::endl;
			}
		}

	public:
		Benchmark()
		{
			benchmark<4>();

			std::cout << std::endl << "Values:" << std::endl;
			std::cout << "Sizes,";
			for (auto s : _sizes)
			{
				std::cout << s << ",";
			}
			std::cout << std::endl;

			std::cout << "Random" << std::endl;
			printBenchmarkMap(_random);

			std::cout << "Ascending" << std::endl;
			printBenchmarkMap(_ascending);

			std::cout << "Descending" << std::endl;
			printBenchmarkMap(_descending);
		}
	};
}