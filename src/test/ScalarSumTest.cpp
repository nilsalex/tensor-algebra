#include "ScalarSum.h"

#include "gtest/gtest.h"

TEST(ScalarSum, DefaultConstructor) {
  ScalarSum ss;

  EXPECT_EQ("0", ss.ToString());

  EXPECT_TRUE(ss.IsZero());
}

TEST(ScalarSum, ConstructorFromScalar) {
  
}
