#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <future>

#include "tests/TestUtil.hpp"
#include "algorithms/algorithms.hpp"

namespace Benchmarks
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

		template<template <typename, size_t> class F, size_t S>
		BenchmarkTime benchmarkSorterWithArray(std::array<double, S> *ary)
		{
			// Create a copy so we do not modify the original
			auto tmp = std::unique_ptr<std::array<double, S>>(new std::array<double, S>(*ary));

			auto start = std::chrono::high_resolution_clock::now();
			F<double, S>::sort(*tmp.get());
			auto end = std::chrono::high_resolution_clock::now();

			auto milliseconds = std::chrono::duration_cast<BenchmarkTime>(end - start);
			
			std::cout << "." << std::flush;
			return milliseconds;
		}

		template<template <typename, size_t> class F, size_t S>
		BenchmarkResult benchmarkSorter(std::array<double, S> *random, std::array<double, S> *ascending, std::array<double, S> *descending)
		{
			// Benchmark for all array types
			auto randomResult = benchmarkSorterWithArray<F, S>(random);
			auto ascendingResult = benchmarkSorterWithArray<F, S>(ascending);
			auto descendingResult = benchmarkSorterWithArray<F, S>(descending);

			// Write the result to our BenchmarkResult
			BenchmarkResult result(F<double, S>::name(), randomResult, ascendingResult, descendingResult);

			return result;
		}

		template<size_t I>
		void benchmarkIteration() {
			// Recursion first, so we get ascending sizes
			benchmarkIteration<I - 1>();
			
			const size_t S = 10000 * (1 << (I - 1));
			std::cout << "Benchmarking for size = " << S << std::flush;
			_sizes.push_back(S);

			// Generate test arrays
			auto random = Tests::generateRandomArray<double, S>();
			auto ascending = Tests::generateAscendingArray<double, S>();
			auto descending = Tests::generateDescendingArray<double, S>();

			// Benchmark sorters
			std::vector<BenchmarkResult> results;
			results.push_back(benchmarkSorter<InsertionSort, S>(random.get(), ascending.get(), descending.get()));
			results.push_back(benchmarkSorter<InsertionSortGuard, S>(random.get(), ascending.get(), descending.get()));
			results.push_back(benchmarkSorter<InsertionSortGuardTransformed, S>(random.get(), ascending.get(), descending.get()));

			// Add results to our map
			for (auto &result : results)
			{
				if (result.name == "")
					continue;

				_random[result.name].push_back(result.random);
				_ascending[result.name].push_back(result.ascending);
				_descending[result.name].push_back(result.descending);
			}

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

	public:
		Benchmark()
		{
			benchmark<0>();

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
	};

	template<>
	void Benchmark::benchmarkIteration<0>()
	{ }
}