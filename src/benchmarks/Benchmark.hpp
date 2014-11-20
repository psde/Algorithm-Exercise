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
		const unsigned int Cycles = 5;

		std::vector<size_t> _sizes;

		template<size_t S>
		using SorterFunction = std::pair<std::string, std::function<void(std::array<double, S>&)>>;

		template<size_t S>
		using SorterVector = std::vector<SorterFunction<S>>;

		using BenchmarkTime = std::chrono::milliseconds;

		using BenchmarkMap = std::unordered_map<std::string, std::vector<BenchmarkTime>>;
		BenchmarkMap _random;
		BenchmarkMap _ascending;
		BenchmarkMap _descending;

		struct BenchmarkResult
		{
			BenchmarkTime random, ascending, descending;

			BenchmarkResult()
				: random(0), ascending(0), descending(0) {}
			BenchmarkResult(BenchmarkTime random, BenchmarkTime ascending, BenchmarkTime descending)
				: random(random), ascending(ascending), descending(descending) {};

			BenchmarkResult& operator+=(const BenchmarkResult &other)
			{
				random += other.random;
				ascending += other.ascending;
				descending += other.descending;
				return *this;
			}
		};

		template<size_t S>
		BenchmarkTime benchmarkSorterWithArray(std::function<void(std::array<double, S>&)> f, std::array<double, S> *ary)
		{
			// Create a copy so we do not modify the original
			auto tmp = std::unique_ptr<std::array<double, S>>(new std::array<double, S>(*ary));

			auto a = tmp.get();
			auto start = std::chrono::high_resolution_clock::now();
			f(*a);
			auto end = std::chrono::high_resolution_clock::now();

			auto milliseconds = std::chrono::duration_cast<BenchmarkTime>(end - start);
			
			return milliseconds;
		}

		template<size_t S>
		BenchmarkResult benchmarkSorter(std::function<void(std::array<double, S>&)> f,
			std::array<double, S> *random, std::array<double, S> *ascending, std::array<double, S> *descending)
		{
			// Benchmark for all array types
			auto randomResult = benchmarkSorterWithArray<S>(f, random);
			auto ascendingResult = benchmarkSorterWithArray<S>(f, ascending);
			auto descendingResult = benchmarkSorterWithArray<S>(f, descending);

			// Write the result to our BenchmarkResult
			BenchmarkResult result(randomResult, ascendingResult, descendingResult);

			std::cout << "." << std::flush;
			return result;
		}

		template<size_t S>
		void buildFunctions(SorterVector<S>&)
		{ }

		template<size_t S, template <typename, size_t> class F, template <typename, size_t> class... args>
		void buildFunctions(SorterVector<S>& functions)
		{
			functions.push_back({ F<double, S>::name(), F<double, S>::sort });
			buildFunctions<S, args...>(functions);
		}

		template<size_t S, template <typename, size_t> class... args>
		SorterVector<S> buildFunctions()
		{
			auto functions = SorterVector<S>();
			buildFunctions<S, args...>(functions);
			return functions;
		}

		template<size_t I>
		void benchmarkIteration() {
			// Recursion first, so we get ascending sizes
			benchmarkIteration<I - 1>();

			const size_t S = 10000 * (1 << (I - 1));
			std::cout << "Benchmarking for size = " << S << std::flush;
			_sizes.push_back(S);

			auto functions = buildFunctions<S,
				InsertionSort, InsertionSortGuard, InsertionSortGuardTransformed,
				QuickSort, ShellSort, MergeSort>();

			std::unordered_map<std::string, BenchmarkResult> results;

			// Generate ascending and descending arrays only once
			auto ascending = Tests::generateAscendingArray<double, S>();
			auto descending = Tests::generateDescendingArray<double, S>();
			for (unsigned int i = 0; i < Cycles; i++)
			{
				/// Generate random array every cycle
				auto random = Tests::generateRandomArray<double, S>();

				for (auto &function : functions)
				{
					auto func = function.second;
					results[function.first] += benchmarkSorter<S>(func, random.get(), ascending.get(), descending.get());
				}
			}

			for (auto &kv : results)
			{
				auto result = kv.second;
				_random[kv.first].push_back(result.random / Cycles);
				_ascending[kv.first].push_back(result.ascending / Cycles);
				_descending[kv.first].push_back(result.descending / Cycles);
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
			benchmark<4>();

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