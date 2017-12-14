#include "TensorMonomial.h"

#include "gtest/gtest.h"

TEST(TensorMonomial, DefaultConstructor) {
  const TensorMonomial tm;

  EXPECT_EQ(0, tm.get_rank());
}

TEST(TensorMonomial, AddFactorRightGetIndexMapping) {
  TensorMonomial tm;
  const Tensor t1 (10, "Hey, I'm a tensor! Yes! Even though my name is quite funny.");
  const Tensor t2 (1, "");

  tm.AddFactorRight(t1);
  const auto im1 = tm.GetIndexMapping(Indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'});

  EXPECT_EQ(10, tm.get_rank());
  EXPECT_EQ(1, im1.size());
  EXPECT_EQ((Indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'}), *(im1.front().first));
  EXPECT_EQ(t1, *(im1.front().second));

  tm.AddFactorRight(t2);
  const auto im2 = tm.GetIndexMapping(Indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'});

  EXPECT_EQ(11, tm.get_rank());
  EXPECT_EQ(2, im2.size());
  EXPECT_EQ((Indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'}), *(im2.front().first));
  EXPECT_EQ(t1, *(im2.front().second));
  EXPECT_EQ((Indices {'k'}), *(im2.at(1).first));
  EXPECT_EQ(t2, *(im2.at(1).second));
}
