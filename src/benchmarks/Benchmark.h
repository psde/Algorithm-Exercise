#pragma once

#include <chrono>

#include "tests/TestUtil.h"
#include "algorithms/insertion_sort.hpp"

namespace benchmarks
{
	class Benchmark
	{
	private:
		template<size_t X, size_t Y>
		struct pow_struct {
			static const size_t value = X * pow_struct<X, Y - 1>::value;
		};

		template<size_t X>
		struct pow_struct<X, 0> {
			static const size_t value = 1;
		};

		template<template <typename, size_t> class F, size_t S>
		static std::chrono::milliseconds foo(std::array<double, S> *ary)
		{
			// Create a copy
			auto tmp = new std::array<double, S>(*ary);

			auto start = std::chrono::steady_clock::now();
			F<double, S>::sort(*tmp);
			auto end = std::chrono::steady_clock::now();

			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			delete tmp;

			std::cout << ".";
			return milliseconds;
		}

		template<size_t N, size_t M>
		struct benchmark_recursion {
			static void go() {

				const size_t S = 100 * pow_struct<2, N>::value;
				std::cout << "Benchmarking for size = " << S;
				auto ary = TestUtil::generateRandomArray<double, S>().release();
				
				foo<InsertionSort, S>(ary);
				foo<InsertionSortGuard, S>(ary);
				foo<InsertionSortGuardTransformed, S>(ary);

				std::cout << " done." << std::endl;
				benchmark_recursion<N + 1, M>::go();
			}
		};

		template<size_t M>
		struct benchmark_recursion<M, M> {
			static void go() {}
		};

		template<size_t M>
		void benchmark() {
			benchmark_recursion<0, M>::go();
		}

	public:
		Benchmark()
		{
			benchmark<3>();
		}
	};
}