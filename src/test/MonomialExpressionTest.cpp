#include "MonomialExpression.h"
#include "TensorMonomial.h"

#include "gtest/gtest.h"

TEST(MonomialExpression, DefaultConstructor) {
  MonomialExpression m;

  EXPECT_EQ("0", m.GetLatexString());
  EXPECT_TRUE(m.IsZero());
}

TEST(MonomialExpression, ConstructorFromTensor) {
  const Tensor t1 (3, "A");
  const Tensor t2 (2, "Y");
  const Tensor t3 (5, "T");

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t2);

  Indices i1 {'a', 'b', 'e'};
  Indices i2 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  const MonomialExpression m1 (t1, i1);
  const MonomialExpression m2 (tm, i2);

  EXPECT_EQ("A^{ a b e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a b c } T^{ d e f g h } Y^{ i j }", m2.GetLatexString());
}

TEST(MonomialExpression, CopyConstructor) {
  const Tensor t1 (3, "A");
  const Tensor t2 (2, "Y");
  const Tensor t3 (5, "T");

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t2);

  Indices i1 {'a', 'b', 'e'};
  Indices i2 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  const MonomialExpression m1 (t1, i1);
  const MonomialExpression m2 (tm, i2);

  const MonomialExpression m1_copy (m1);
  const MonomialExpression m2_copy (m2);

  EXPECT_EQ("A^{ a b e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a b c } T^{ d e f g h } Y^{ i j }", m2.GetLatexString());

  EXPECT_EQ("A^{ a b e }", m1_copy.GetLatexString());
  EXPECT_EQ("A^{ a b c } T^{ d e f g h } Y^{ i j }", m2_copy.GetLatexString());
}

TEST(MonomialExpression, ApplySymmetries) {
  Tensor t1 (3, "A");
  Tensor t2 (2, "Y");
  Tensor t3 (5, "T");

  t1.SetSymmetric();
  t2.SetAntisymmetric();

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t2);

  Indices i1 {'b', 'a', 'e'};
  Indices i2 {'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'j', 'i'};

  MonomialExpression m1 (t1, i1);
  MonomialExpression m2 (tm, i2);

  EXPECT_EQ("A^{ b a e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a c b } T^{ d e f g h } Y^{ j i }", m2.GetLatexString());

  m1.ApplySymmetries();
  m2.ApplySymmetries();

  EXPECT_EQ("A^{ a b e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a b c } T^{ d e f g h } Y^{ i j }", m2.GetLatexString());
}

TEST(MonomialExpression, Sort) {
  Tensor t1 (3, "A");
  Tensor t2 (2, "Y");
  Tensor t3 (5, "T");
  Tensor t4 (5, "T");

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t2);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t4);

  Indices i1 {'b', 'a', 'e'};
  Indices i2 {'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'o', 'p', 'm', 'n', 'j', 'i', 'q'};

  MonomialExpression m1 (t1, i1);
  MonomialExpression m2 (tm, i2);

  m1.Sort();
  m2.Sort();

  EXPECT_EQ("A^{ b a e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a c b } T^{ f g h o p } T^{ m n j i q } Y^{ d e }", m2.GetLatexString());
}

TEST(MonomialExpression, ExchangeIndices) {
  Tensor t1 (3, "A");
  Tensor t2 (2, "Y");
  Tensor t3 (5, "T");
  Tensor t4 (5, "T");

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t2);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t4);

  Indices i1       {'b', 'a', 'e'};
  Indices i1_new   {'b', 'e', 'a'};
  Indices i2       {'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'o', 'p', 'm', 'n', 'j', 'i', 'q'};
  Indices i2_new   {'a', 'q', 'n', 'd', 'e', 'f', 'g', 'h', 'o', 'p', 'm', 'c', 'j', 'i', 'b'};

  MonomialExpression m1 (t1, i1);
  MonomialExpression m2 (tm, i2);

  m1.ExchangeIndices(i1, i1_new);
  m2.ExchangeIndices(i2, i2_new);

  EXPECT_EQ("A^{ b e a }", m1.GetLatexString());
  EXPECT_EQ("A^{ a q n } Y^{ d e } T^{ f g h o p } T^{ m c j i b }", m2.GetLatexString());
}

TEST(MonomialExpression, EvaluateIndices) {
  Tensor eta      (2, "eta");
  Tensor epsilon  (4, "epsilon");

  eta.SetSymmetric();
  epsilon.SetAntisymmetric();

  TensorMonomial tm1;
  tm1.AddFactorRight (eta);
  MonomialExpression m1 (eta, Indices {'a', 'b'});

  EXPECT_EQ(Rational(1, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}}) );
  EXPECT_EQ(Rational(-1, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}}) );
  EXPECT_EQ(Rational(-1, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}}) );
  EXPECT_EQ(Rational(0, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}}) );
  EXPECT_EQ(Rational(-1, 1), m1.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}}) );

  TensorMonomial tm2;
  tm2.AddFactorRight (epsilon);
  MonomialExpression m2 (epsilon, Indices {'a', 'b', 'i', 'z'});

  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 1}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 2}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 3}, {Index::i, 3}, {Index::z, 3}}) );

  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 0}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 2}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 3}, {Index::i, 3}, {Index::z, 3}}) );

  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 0}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 1}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 3}, {Index::i, 3}, {Index::z, 3}}) );

  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 0}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(-1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(1, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 2}, {Index::i, 3}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 0}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 0}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 0}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 0}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 1}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 1}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 1}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 1}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 2}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 2}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 2}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 2}, {Index::z, 3}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 3}, {Index::z, 0}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 3}, {Index::z, 1}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 3}, {Index::z, 2}}) );
  EXPECT_EQ(Rational(0, 1), m2.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 3}, {Index::i, 3}, {Index::z, 3}}) );

  TensorMonomial tm3;
  tm3.AddFactorRight (eta);
  tm3.AddFactorRight (eta);
  MonomialExpression m3 (tm3, Indices {'a', 'b', 'c', 'd'});

  EXPECT_EQ(Rational(1, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::c, 0}, {Index::d, 0}}) );
  EXPECT_EQ(Rational(-1, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::c, 1}, {Index::d, 1}}) );
  EXPECT_EQ(Rational(1, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 1}, {Index::b, 1}, {Index::c, 1}, {Index::d, 1}}) );
  EXPECT_EQ(Rational(0, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 0}, {Index::b, 0}, {Index::c, 2}, {Index::d, 0}}) );
  EXPECT_EQ(Rational(0, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 3}, {Index::b, 1}, {Index::c, 1}, {Index::d, 0}}) );
  EXPECT_EQ(Rational(1, 1), m3.EvaluateIndices (std::map<Index, size_t> {{Index::a, 2}, {Index::b, 2}, {Index::c, 3}, {Index::d, 3}}) );
}
