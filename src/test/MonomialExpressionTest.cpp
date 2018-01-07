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
  Indices i3 {'a', 'c', 'b', 'd', 'e', 'm', 'n', 'j', 'i', 'q', 'f', 'g', 'h', 'o', 'p'};

  MonomialExpression m1 (t1, i1);
  MonomialExpression m2 (tm, i2);
  MonomialExpression m3 (tm, i3);

  m1.Sort();
  m2.Sort();
  m3.Sort();

  EXPECT_EQ("A^{ b a e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a c b } T^{ f g h o p } T^{ m n j i q } Y^{ d e }", m2.GetLatexString());
  EXPECT_EQ("A^{ a c b } T^{ f g h o p } T^{ m n j i q } Y^{ d e }", m3.GetLatexString());
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

TEST(MonomialExpression, MultiplyOther) {
  Tensor A (3, "A");
  Tensor B (2, "B");
  Tensor C (5, "C");
  Tensor D (4, "D");

  TensorMonomial tm1;
  TensorMonomial tm2;
  TensorMonomial tm3;

  tm1.AddFactorRight(A);

  tm2.AddFactorRight(B);

  tm3.AddFactorRight(C);
  tm3.AddFactorRight(D);

  Indices i1 {'a', 'x', 'z'};
  Indices i2 {'x', 'y'};
  Indices i3 {'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  MonomialExpression m1 (tm1, i1);
  MonomialExpression m2 (tm2, i2);
  MonomialExpression m3 (tm3, i3);

  EXPECT_EQ("A^{ a x z }", m1.GetLatexString());
  EXPECT_EQ("B^{ x y }", m2.GetLatexString());
  EXPECT_EQ("C^{ b c d e f } D^{ g h i j }", m3.GetLatexString());

  MonomialExpression m11 = m1.MultiplyOther(m1);
  MonomialExpression m12 = m1.MultiplyOther(m2);
  MonomialExpression m13 = m1.MultiplyOther(m3);
  MonomialExpression m21 = m2.MultiplyOther(m1);
  MonomialExpression m22 = m2.MultiplyOther(m2);
  MonomialExpression m23 = m2.MultiplyOther(m3);
  MonomialExpression m31 = m3.MultiplyOther(m1);
  MonomialExpression m32 = m3.MultiplyOther(m2);
  MonomialExpression m33 = m3.MultiplyOther(m3);

  EXPECT_EQ("A^{ a x z }", m1.GetLatexString());
  EXPECT_EQ("B^{ x y }", m2.GetLatexString());
  EXPECT_EQ("C^{ b c d e f } D^{ g h i j }", m3.GetLatexString());

  EXPECT_EQ("A^{ a x z } A^{ a x z }", m11.GetLatexString());
  EXPECT_EQ("A^{ a x z } B^{ x y }", m12.GetLatexString());
  EXPECT_EQ("A^{ a x z } C^{ b c d e f } D^{ g h i j }", m13.GetLatexString());

  EXPECT_EQ("B^{ x y } A^{ a x z }", m21.GetLatexString());
  EXPECT_EQ("B^{ x y } B^{ x y }", m22.GetLatexString());
  EXPECT_EQ("B^{ x y } C^{ b c d e f } D^{ g h i j }", m23.GetLatexString());

  EXPECT_EQ("C^{ b c d e f } D^{ g h i j } A^{ a x z }", m31.GetLatexString());
  EXPECT_EQ("C^{ b c d e f } D^{ g h i j } B^{ x y }", m32.GetLatexString());
  EXPECT_EQ("C^{ b c d e f } D^{ g h i j } C^{ b c d e f } D^{ g h i j }", m33.GetLatexString());

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

TEST(MonomialExpression, GetLatexString) {
  const Tensor t1 (3, "A");
  const Tensor t2 (2, "Y");
  const Tensor t3 (5, "the ting goes skrrra");
  const Tensor t4 (0, "");

  TensorMonomial tm;
  tm.AddFactorRight(t1);
  tm.AddFactorRight(t3);
  tm.AddFactorRight(t2);
  tm.AddFactorRight(t4);

  Indices i1 {'a', 'b', 'e'};
  Indices i2 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  const MonomialExpression m1 (t1, i1);
  const MonomialExpression m2 (tm, i2);

  EXPECT_EQ("A^{ a b e }", m1.GetLatexString());
  EXPECT_EQ("A^{ a b c } the ting goes skrrra^{ d e f g h } Y^{ i j } ^{ }", m2.GetLatexString());

  EXPECT_EQ("A^{ a b e }", m1.GetLatexString(true));
  EXPECT_EQ("A^{ a b c } the ting goes skrrra^{ d e f g h } Y^{ i j } ^{ }", m2.GetLatexString(true));

  EXPECT_EQ("A_{ a b e }", m1.GetLatexString(false));
  EXPECT_EQ("A_{ a b c } the ting goes skrrra_{ d e f g h } Y_{ i j } _{ }", m2.GetLatexString(false));
}

TEST(MonomialExpression, equals) {
  const Tensor t1 (3, "A");
  const Tensor t2 (2, "Y");
  const Tensor t3 (5, "the ting goes skrrra");
  const Tensor t4 (0, "");
  const Tensor t5 (3, "A");
  const Tensor t6 (2, "Y");
  const Tensor t7 (5, "the ting goes skrrra");
  const Tensor t8 (0, "");

  TensorMonomial tm1;
  tm1.AddFactorRight(t1);
  tm1.AddFactorRight(t3);
  tm1.AddFactorRight(t2);
  tm1.AddFactorRight(t4);

  TensorMonomial tm2;
  tm2.AddFactorRight(t5);
  tm2.AddFactorRight(t7);
  tm2.AddFactorRight(t6);
  tm2.AddFactorRight(t8);

  Indices i1 {'a', 'b', 'e'};
  Indices i2 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
  Indices i3 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  const MonomialExpression m1 (t1, i1);
  const MonomialExpression m2 (tm1, i2);
  const MonomialExpression m3 (tm2, i3);
  
  EXPECT_EQ(m1, m1);
  EXPECT_NE(m1, m2);
  EXPECT_NE(m1, m3);

  EXPECT_NE(m2, m1);
  EXPECT_EQ(m2, m2);
  EXPECT_EQ(m2, m3);

  EXPECT_NE(m3, m1);
  EXPECT_EQ(m3, m2);
  EXPECT_EQ(m3, m3);
}

TEST(MonomialExpression, lessThan) {
  const Tensor t1 (3, "A");
  const Tensor t2 (2, "Y");
  const Tensor t3 (5, "the ting goes skrrra");
  const Tensor t4 (0, "");
  const Tensor t5 (3, "A");
  const Tensor t6 (2, "Y");
  const Tensor t7 (5, "the ting goes skrrra");

  TensorMonomial tm1;
  tm1.AddFactorRight(t1);
  tm1.AddFactorRight(t3);
  tm1.AddFactorRight(t2);
  tm1.AddFactorRight(t4);

  TensorMonomial tm2;
  tm2.AddFactorRight(t5);
  tm2.AddFactorRight(t7);
  tm2.AddFactorRight(t6);

  Indices i1 {'a', 'e'};
  Indices i2 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
  Indices i3 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  const MonomialExpression m1 (t2, i1);
  const MonomialExpression m2 (tm1, i2);
  const MonomialExpression m3 (tm2, i3);
  
  EXPECT_FALSE(m1 < m1);
  EXPECT_FALSE(m1 < m2);
  EXPECT_FALSE(m1 < m3);
  
  EXPECT_TRUE(m2 < m1);
  EXPECT_FALSE(m2 < m2);
  EXPECT_FALSE(m2 < m3);
  
  EXPECT_TRUE(m3 < m1);
  EXPECT_TRUE(m3 < m2);
  EXPECT_FALSE(m3 < m3);
}
