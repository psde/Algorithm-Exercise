#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <future>

#include "tests/TestUtil.h"
#include "algorithms/insertion_sort.hpp"

namespace benchmarks
{
	class Benchmark
	{
	private:

		std::vector<size_t> _sizes;

		using BenchmarkTime = std::chrono::milliseconds;

		using BenchmarkMap = std::unordered_map<std::string, std::vector<BenchmarkTime>>;
		BenchmarkMap _random;
		BenchmarkMap _ascending;
		BenchmarkMap _descending;

		struct BenchmarkResult
		{
			std::string name;
			BenchmarkTime random, ascending, descending;

			BenchmarkResult()
				: name("") {}
			BenchmarkResult(std::string name, BenchmarkTime random, BenchmarkTime ascending, BenchmarkTime descending)
				: name(name), random(random), ascending(ascending), descending(descending) {};
		};

		template<size_t X, size_t Y>
		struct pow_struct {
			static const size_t value = X * pow_struct<X, Y - 1>::value;
		};

		template<size_t X>
		struct pow_struct<X, 0> {
			static const size_t value = 1;
		};

		template<template <typename, size_t> class F, size_t S>
		BenchmarkTime benchmarkSorterWithArray(std::array<double, S> *ary)
		{
			// Create a copy so we do not modify the original
			auto tmp = new std::array<double, S>(*ary);

			auto start = std::chrono::high_resolution_clock::now();
			F<double, S>::sort(*tmp);
			auto end = std::chrono::high_resolution_clock::now();

			auto milliseconds = std::chrono::duration_cast<BenchmarkTime>(end - start);

			delete tmp;

			std::cout << "." << std::flush;
			return milliseconds;
		}

		template<template <typename, size_t> class F, size_t S>
		BenchmarkResult benchmarkSorter(std::array<double, S> *random, std::array<double, S> *ascending, std::array<double, S> *descending)
		{
			// Construct a future and launch async
			// Due to a VS13 compiler bug a lambda is needed
			auto randomResult(std::async(std::launch::async, [&]() { return benchmarkSorterWithArray<F, S>(random); }));
			auto ascendingResult(std::async(std::launch::async, [&]() { return benchmarkSorterWithArray<F, S>(ascending); }));
			auto descendingResult(std::async(std::launch::async, [&]() { return benchmarkSorterWithArray<F, S>(descending); }));

			// Write the result to our BenchmarkResult
			BenchmarkResult result(F<double, S>::name(), randomResult.get(), ascendingResult.get(), descendingResult.get());

			return result;
		}

		template<size_t I>
		void benchmarkIteration() {
			// Recursion first, so we get ascending sizes
			benchmarkIteration<I - 1>();
			
			const size_t S = 10000 * pow_struct<2, I - 1>::value;
			std::cout << "Benchmarking for size = " << S << std::flush;
			_sizes.push_back(S);

			// Generate test arrays
			auto random = TestUtil::generateRandomArray<double, S>().release();
			auto ascending = TestUtil::generateAscendingArray<double, S>().release();
			auto descending = TestUtil::generateDescendingArray<double, S>().release();

			// Generate futures for all sorters
			std::vector<std::future<BenchmarkResult>> futures;

			// Due to a VS13 compiler bug a lambda is needed
			// Due to another VS13 compiler bug 'S' must be defined inside the lambda, otherwise VS13 does not think it is a compile-time constant
			futures.push_back(std::async(std::launch::async, [&]() { const size_t S = 10000 * pow_struct<2, I - 1>::value; return benchmarkSorter<InsertionSort, S>(random, ascending, descending); }));
			futures.push_back(std::async(std::launch::async, [&]() { const size_t S = 10000 * pow_struct<2, I - 1>::value; return benchmarkSorter<InsertionSortGuard, S>(random, ascending, descending); }));
			futures.push_back(std::async(std::launch::async, [&]() { const size_t S = 10000 * pow_struct<2, I - 1>::value; return benchmarkSorter<InsertionSortGuardTransformed, S>(random, ascending, descending); }));

			// Add results to our map
			for (auto &future : futures)
			{
				BenchmarkResult result = future.get();
				if (result.name == "")
					continue;

				_random[result.name].push_back(result.random);
				_ascending[result.name].push_back(result.ascending);
				_descending[result.name].push_back(result.descending);
			}

			// Delete test arrays
			delete random;
			delete ascending;
			delete descending;

			std::cout << " done." << std::endl;
		}

		template<size_t I>
		void benchmark() {
			benchmarkIteration<I>();
		}

		void printBenchmarkMap(std::ostream& stream, BenchmarkMap &map)
		{
			for (const auto &kv : map)
			{
				stream << kv.first << ",";

				for (const auto &ms : kv.second)
				{
					stream << ms.count() << ",";
				}
				stream << std::endl;
			}
		}

	public:
		Benchmark()
		{
			benchmark<5>();

			std::cout << std::endl << "Values:" << std::endl;
			printResults(std::cout);

			std::cout << std::endl << "Writing to results.csv" << std::endl;
			std::ofstream fs("results.csv", std::ios::out | std::ios::trunc);
			if (!fs)
			{
				std::cerr << "Could not open file" << std::endl;
				return;
			}
			printResults(fs);
			fs.close();
		}

		void printResults(std::ostream& stream)
		{
			stream << "Sizes,";
			for (auto s : _sizes)
			{
				stream << s << ",";
			}
			stream << std::endl;

			stream << "Random" << std::endl;
			printBenchmarkMap(stream, _random);

			stream << "Ascending" << std::endl;
			printBenchmarkMap(stream, _ascending);

			stream << "Descending" << std::endl;
			printBenchmarkMap(stream, _descending);
		}
	};

	template<>
	void Benchmark::benchmarkIteration<0>()
	{ }
}