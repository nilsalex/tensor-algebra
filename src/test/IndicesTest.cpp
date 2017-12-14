#include "Indices.h"

#include "gtest/gtest.h"

TEST(Indices, DefaultConstructor) {
  const Indices indices;

  EXPECT_EQ(0, indices.size());
  EXPECT_EQ("{ }", indices.ToString());
}

TEST(Indices, ConstructorFromEnum) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::z, Index::z };

  EXPECT_EQ(0, indices1.size());
  EXPECT_EQ(3, indices2.size());
  EXPECT_EQ(2, indices3.size());

  EXPECT_EQ("{ }", indices1.ToString());
  EXPECT_EQ("{ a y g }", indices2.ToString());
  EXPECT_EQ("{ z z }", indices3.ToString());
}

TEST(Indices, ConstructorFromChar) {
  const Indices indices1 { };
  const Indices indices2 { 'a', 'y', 'g' };
  const Indices indices3 { 'z', 'z' };

  EXPECT_EQ(0, indices1.size());
  EXPECT_EQ(3, indices2.size());
  EXPECT_EQ(2, indices3.size());

  EXPECT_EQ("{ }", indices1.ToString());
  EXPECT_EQ("{ a y g }", indices2.ToString());
  EXPECT_EQ("{ z z }", indices3.ToString());
}

TEST(Indices, CopyConstructor) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::z, Index::z };

  const Indices indices1_copy (indices1);
  const Indices indices2_copy (indices2);
  const Indices indices3_copy (indices3);

  EXPECT_EQ(0, indices1.size());
  EXPECT_EQ(3, indices2.size());
  EXPECT_EQ(2, indices3.size());

  EXPECT_EQ("{ }", indices1.ToString());
  EXPECT_EQ("{ a y g }", indices2.ToString());
  EXPECT_EQ("{ z z }", indices3.ToString());

  EXPECT_EQ("{ }", indices1_copy.ToString());
  EXPECT_EQ("{ a y g }", indices2_copy.ToString());
  EXPECT_EQ("{ z z }", indices3_copy.ToString());
}

TEST(Indices, ConstructorFromIndices) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };

  const Indices indices1_new (indices1, { });
  const Indices indices2_new (indices2, {{Index::a, Index::a}, {Index::g, Index::c}, {Index::y, Index::b}});

  const Indices indices1_new_comp;
  const Indices indices2_new_comp {Index::a, Index::b, Index::c};

  EXPECT_EQ(indices1_new, indices1_new_comp);
  EXPECT_EQ(indices2_new, indices2_new_comp);
}

TEST(Indices, SortAndMakeUnique) {
  Indices indices1 { };
  Indices indices2 { Index::a, Index::y, Index::g };
  Indices indices3 { Index::z, Index::z };

  indices1.SortAndMakeUnique();
  indices2.SortAndMakeUnique();
  indices3.SortAndMakeUnique();

  EXPECT_EQ(indices1, (Indices()));
  EXPECT_EQ(indices2, (Indices {Index::a, Index::g, Index::y}));
  EXPECT_EQ(indices3, (Indices {Index::z}));
}

TEST(Indices, ToString) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::z, Index::z };

  EXPECT_EQ("{ }", indices1.ToString());
  EXPECT_EQ("{ a y g }", indices2.ToString());
  EXPECT_EQ("{ z z }", indices3.ToString());
}

TEST(Indices, size) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::z, Index::z };

  EXPECT_EQ(0, indices1.size());
  EXPECT_EQ(3, indices2.size());
  EXPECT_EQ(2, indices3.size());
}

TEST(Indices, at) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::z, Index::z };

  EXPECT_EQ(Index::a, indices2.at(0));
  EXPECT_EQ(Index::y, indices2.at(1));
  EXPECT_EQ(Index::g, indices2.at(2));

  EXPECT_EQ(Index::z, indices3.at(0));
  EXPECT_EQ(Index::z, indices3.at(1));
}

TEST(Indices, Sort) {
  Indices indices1 { };
  Indices indices2 { Index::a, Index::y, Index::g };
  Indices indices3 { Index::a, Index::y, Index::z, Index::g };

  EXPECT_TRUE(indices1.Sort());
  EXPECT_FALSE(indices2.Sort());
  EXPECT_TRUE(indices3.Sort());

  EXPECT_EQ(indices1, (Indices()));
  EXPECT_EQ(indices2, (Indices {Index::a, Index::g, Index::y}));
  EXPECT_EQ(indices3, (Indices {Index::a, Index::g, Index::y, Index::z}));
}

TEST(Indices, Permutation) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::a, Index::y, Index::z, Index::g };
  const Indices indices4 { Index::z, Index::z };
  
  const Indices indices1_perm = indices1.Permutation(std::vector<size_t> {});
  const Indices indices2_perm = indices2.Permutation(std::vector<size_t> {2, 0, 1});
  const Indices indices3_perm = indices3.Permutation(std::vector<size_t> {2, 0, 1, 3});
  const Indices indices4_perm = indices4.Permutation(std::vector<size_t> {1, 0});

  EXPECT_EQ(indices1_perm, (Indices()));
  EXPECT_EQ(indices2_perm, (Indices {Index::g, Index::a, Index::y}));
  EXPECT_EQ(indices3_perm, (Indices {Index::z, Index::a, Index::y, Index::g}));
  EXPECT_EQ(indices4_perm, (Indices {Index::z, Index::z}));
}

TEST(Indices, SubIndices) {
  const Indices indices { Index::a, Index::y, Index::g, Index::f };

  const Indices indices_sub1 = indices.SubIndices(0, 4);
  const Indices indices_sub2 = indices.SubIndices(2, 0);
  const Indices indices_sub3 = indices.SubIndices(1, 1);
  const Indices indices_sub4 = indices.SubIndices(1, 3);

  EXPECT_EQ(indices_sub1, (Indices { Index::a, Index::y, Index::g, Index::f }));
  EXPECT_EQ(indices_sub2, (Indices()));
  EXPECT_EQ(indices_sub3, (Indices { Index::y }));
  EXPECT_EQ(indices_sub4, (Indices { Index::y, Index::g, Index::f }));
}

TEST(Indices, equals) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::a, Index::y, Index::z, Index::g };

  EXPECT_EQ(indices1, indices1);
  EXPECT_NE(indices1, indices2);
  EXPECT_NE(indices1, indices3);
  EXPECT_NE(indices2, indices1);
  EXPECT_EQ(indices2, indices2);
  EXPECT_NE(indices2, indices3);
  EXPECT_NE(indices3, indices1);
  EXPECT_NE(indices3, indices2);
  EXPECT_EQ(indices3, indices3);
}

TEST(Indices, lessThan) {
  const Indices indices1 { };
  const Indices indices2 { Index::a, Index::y, Index::g };
  const Indices indices3 { Index::a, Index::y, Index::z, Index::g };
  const Indices indices4 { Index::a, Index::x, Index::z, Index::g };

  EXPECT_FALSE(indices1 < indices1);
  EXPECT_TRUE(indices1 < indices2);
  EXPECT_TRUE(indices1 < indices3);
  EXPECT_TRUE(indices1 < indices4);
  EXPECT_FALSE(indices2 < indices1);
  EXPECT_FALSE(indices2 < indices2);
  EXPECT_TRUE(indices2 < indices3);
  EXPECT_FALSE(indices2 < indices4);
  EXPECT_FALSE(indices3 < indices1);
  EXPECT_FALSE(indices3 < indices2);
  EXPECT_FALSE(indices3 < indices3);
  EXPECT_FALSE(indices3 < indices4);
  EXPECT_FALSE(indices4 < indices1);
  EXPECT_TRUE(indices4 < indices2);
  EXPECT_TRUE(indices4 < indices3);
  EXPECT_FALSE(indices4 < indices4);
}
