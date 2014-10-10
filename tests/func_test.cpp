#include "gtest/gtest.h"
#include "func.h"

TEST(FuncTest, IsWorking) {
  EXPECT_EQ(23, func::test());
}