#include <iostream>
#include <random>

#include "insertion_sort.hpp"

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> f_rand(0.f, 1000.f);

	std::array<float, 5> array;
	for (auto &f : array)
	{
		f = f_rand(gen);
		std::cout << f << std::endl;
	}
	
	insertion_sort::sort<float, 5>(array);

	std::cout << "sorted:" << std::endl;
	for (auto f : array)
	{
		std::cout << f << std::endl;
	}

	getchar();
}