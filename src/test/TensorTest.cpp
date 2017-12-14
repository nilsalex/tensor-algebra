#include "Tensor.h"

#include "gtest/gtest.h"

TEST(Tensor, DefaultConstructor) {
  const Tensor t;
 
  EXPECT_EQ("", t.get_name());
  EXPECT_EQ(0, t.get_rank());
}

TEST(Tensor, ConstructorFromRankName) {
  const Tensor t (5, "5");

  EXPECT_EQ("5", t.get_name());
  EXPECT_EQ(5, t.get_rank());
}

TEST(Tensor, CopyConstructor) {
  const Tensor t1;
  const Tensor t2 (7, "TenSoRR");

  const Tensor u1 (t1);
  const Tensor u2 (t2);

  EXPECT_EQ("", t1.get_name());
  EXPECT_EQ("", u1.get_name());
  EXPECT_EQ("TenSoRR", t2.get_name());
  EXPECT_EQ("TenSoRR", u2.get_name());

  EXPECT_EQ(0, t1.get_rank());
  EXPECT_EQ(0, u1.get_rank());
  EXPECT_EQ(7, t2.get_rank());
  EXPECT_EQ(7, u2.get_rank());
}

TEST(Tensor, Antisymmetric) {
  Tensor t1 (99, "Hey!##");

  EXPECT_FALSE(t1.IsAntisymmetric());
  EXPECT_FALSE(t1.IsSymmetric());

  t1.SetAntisymmetric();

  EXPECT_TRUE(t1.IsAntisymmetric());
  EXPECT_FALSE(t1.IsSymmetric());
}

TEST(Tensor, Symmetric) {
  Tensor t1 (99, "Hey!##");

  EXPECT_FALSE(t1.IsAntisymmetric());
  EXPECT_FALSE(t1.IsSymmetric());

  t1.SetSymmetric();

  EXPECT_FALSE(t1.IsAntisymmetric());
  EXPECT_TRUE(t1.IsSymmetric());
}

TEST(Tensor, GetIndexMapping) {
  const Tensor t1;
  const Tensor t2 (11, "Whaaa");

  const Indices i1;
  const Indices i2 {'a', 'b', 'c', 'e', 'j', 'k', 'l', 'm', 'o', 'p', 'z'};

  const auto im1 = t1.GetIndexMapping(i1);
  const auto im2 = t2.GetIndexMapping(i2);

  EXPECT_EQ(1, im1.size());
  EXPECT_EQ(1, im1.size());

  EXPECT_EQ(*(im1.front().first), i1);
  EXPECT_EQ(*(im2.front().first), i2);

  EXPECT_EQ(*(im1.front().second), t1);
  EXPECT_EQ(*(im2.front().second), t2);
}

TEST(Tensor, equals) {
  const Tensor t1;
  const Tensor t2 (11, "Whaaa");
  const Tensor t3 (11, "XyYYz");
  const Tensor t4 (1, "XyYYz");

  EXPECT_EQ(t1, t1);
  EXPECT_NE(t1, t2);
  EXPECT_NE(t1, t3);
  EXPECT_NE(t1, t4);
  EXPECT_NE(t2, t1);
  EXPECT_EQ(t2, t2);
  EXPECT_NE(t2, t3);
  EXPECT_NE(t2, t4);
  EXPECT_NE(t3, t1);
  EXPECT_NE(t3, t2);
  EXPECT_EQ(t3, t3);
  EXPECT_NE(t3, t4);
  EXPECT_NE(t4, t1);
  EXPECT_NE(t4, t2);
  EXPECT_NE(t4, t3);
  EXPECT_EQ(t4, t4);
}

TEST(Tensor, lessThan) {
  const Tensor t1;
  const Tensor t2 (11, "Whaaa");
  const Tensor t3 (11, "XyYYz");
  const Tensor t4 (1, "XyYYz");

  EXPECT_FALSE(t1 < t1);
  EXPECT_TRUE(t1 < t2);
  EXPECT_TRUE(t1 < t3);
  EXPECT_TRUE(t1 < t4);
  EXPECT_FALSE(t2 < t1);
  EXPECT_FALSE(t2 < t2);
  EXPECT_TRUE(t2 < t3);
  EXPECT_TRUE(t2 < t4);
  EXPECT_FALSE(t3 < t1);
  EXPECT_FALSE(t3 < t2);
  EXPECT_FALSE(t3 < t3);
  EXPECT_FALSE(t3 < t4);
  EXPECT_FALSE(t4 < t1);
  EXPECT_FALSE(t4 < t2);
  EXPECT_TRUE(t4 < t3);
  EXPECT_FALSE(t4 < t4);
}
