#include <iostream>
#include <random>

#include "gtest/gtest.h"
#include "algorithms/insertion_sort.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}