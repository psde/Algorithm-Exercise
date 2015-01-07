#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
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

		using BenchmarkMap = std::map<std::string, std::vector<BenchmarkTime>>;
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
		void trashCache()
		{
			auto ary = std::unique_ptr<std::array<double, S>>(new std::array<double, S>());

			volatile double tmp = 0;
			for (volatile int i = 0; i < S; i++)
			{
				tmp = i;
				(*ary)[i] = tmp;
				(*ary)[i] *= 42;
				tmp = (*ary)[i];
			}
		}

		template<size_t S>
		BenchmarkTime benchmarkSorterWithArray(std::function<void(std::array<double, S>&)> f, std::array<double, S> *ary)
		{
			// Create a copy so we do not modify the original
			auto tmp = std::unique_ptr<std::array<double, S>>(new std::array<double, S>(*ary));

			auto a = tmp.get();
			trashCache<S>();
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
				QuickSort, QuickSortShift, QuickSortShift3Way, QuickSort3WayHybrid,
				MergeSortTopDown, MergeSortBottomUp, MergeSortNatural, 
				ShellSort>();

			std::unordered_map<std::string, BenchmarkResult> results;

			// Generate ascending and descending arrays only once
			auto ascending = Tests::TestArrays<double, S>::generateAscendingArray();
			auto descending = Tests::TestArrays<double, S>::generateDescendingArray();
			for (unsigned int i = 0; i < Cycles; i++)
			{
				/// Generate random array every cycle
				auto random = Tests::TestArrays<double, S>::generateRandomArray();

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

		void printCSVMap(std::ostream& stream, BenchmarkMap &map)
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

		void printCSV(std::ostream& stream)
		{
			stream << "Sizes,";
			for (auto s : _sizes)
			{
				stream << s << ",";
			}
			stream << std::endl;

			stream << "Random" << std::endl;
			printCSVMap(stream, _random);

			stream << "Ascending" << std::endl;
			printCSVMap(stream, _ascending);

			stream << "Descending" << std::endl;
			printCSVMap(stream, _descending);
		}

		void printLatexTable(std::ostream& stream, BenchmarkMap &map, std::string caption)
		{
			stream << "\\begin{table}[h]" << std::endl;
			stream << "\\begin{tabular}{|l|";
			for (const auto &kv : map)
			{
				for (const auto &ms : kv.second)
				{
					stream << "l|";
				}
				break;
			}
			stream << "}" << std::endl;
			stream << "\\hline" << std::endl;

			for (auto s : _sizes)
			{
				stream << " & "<< s;
			}
			stream << " \\\\ \\hline" << std::endl;

			for (const auto &kv : map)
			{
				stream << kv.first;
				for (const auto &ms : kv.second)
				{
					stream << " & " << ms.count();
				}
				stream << " \\\\ \\hline" << std::endl;
			}

			stream << "\\end{tabular}" << std::endl;
			stream << "\\caption{" << caption << "}" << std::endl;
			stream << "\\end{table}" << std::endl;
		}		
		
		void printLatex(std::ostream& stream)
		{
			printLatexTable(stream, _random, "Random");
			printLatexTable(stream, _ascending, "Ascending");
			printLatexTable(stream, _descending, "Descending");
		}

	public:
		Benchmark()
		{
			benchmark<1>();
			std::cout << std::endl << "Values:" << std::endl;
			printCSV(std::cout);

			std::cout << std::endl << "Writing to benchmark.tex" << std::endl;
			std::ofstream fs("benchmark.tex", std::ios::out | std::ios::trunc);
			if (!fs)
			{
				std::cerr << "Could not open file" << std::endl;
				return;
			}
			printLatex(fs);
			fs.close();
		}
	};

	template<>
	void Benchmark::benchmarkIteration<0>()
	{ }
}