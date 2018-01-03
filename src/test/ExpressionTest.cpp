#include "Expression.h"
#include "TensorMonomial.h"

#include "gtest/gtest.h"

TEST(Expression, DefaultConstructor) {
  const Expression e;

  EXPECT_EQ("0", e.GetLatexString());
  EXPECT_TRUE(e.IsZero());
}

TEST(Expression, CopyConstructor) {
  const Tensor eta (2, "eta");
  const Tensor epsilon (4, "epsilon");
  const Tensor v (1, "v");
  const Tensor s (3, "s");

  const Indices i { 'a', 'b', 'y', 'k', 'l', 'n' };

  TensorMonomial tm1;
  TensorMonomial tm2;

  tm1.AddFactorRight(eta);
  tm1.AddFactorRight(s);
  tm1.AddFactorRight(v);

  tm2.AddFactorRight(epsilon);
  tm2.AddFactorRight(v);
  tm2.AddFactorRight(v);

  const MonomialExpression m1 (tm1, i);
  const MonomialExpression m2 (tm2, i);

  Expression e;

  e.AddSummand(m1, Rational(7, 2));
  e.AddSummand(m2, Scalar(Rational(9, 3), 8));

  Expression e_copy(e);

  EXPECT_EQ("7/2 eta^{ a b } s^{ y k l } v^{ n }\n+ 3*e_8 epsilon^{ a b y k } v^{ l } v^{ n }",      e.GetLatexString());
  EXPECT_EQ("7/2 eta^{ a b } s^{ y k l } v^{ n }\n+ 3*e_8 epsilon^{ a b y k } v^{ l } v^{ n }", e_copy.GetLatexString());
}

TEST(Expression, AddSummand) {
  const Tensor eta (2, "eta");
  const Tensor epsilon (4, "epsilon");
  const Tensor v (1, "v");
  const Tensor s (3, "s");

  const Indices i { 'a', 'b', 'y', 'k', 'l', 'n' };

  TensorMonomial tm1;
  TensorMonomial tm2;

  tm1.AddFactorRight(eta);
  tm1.AddFactorRight(s);
  tm1.AddFactorRight(v);

  tm2.AddFactorRight(epsilon);
  tm2.AddFactorRight(v);
  tm2.AddFactorRight(v);

  const MonomialExpression m1 (tm1, i);
  const MonomialExpression m2 (tm2, i);

  Expression e1;

  e1.AddSummand(m1);
  e1.AddSummand(m2);

  Expression e2;

  e2.AddSummand(m1, Rational(7, 2));
  e2.AddSummand(m2, Scalar(Rational(9, 3), 8));

  Expression e3;

  e3.AddSummand(m1, 7);
  e3.AddSummand(m2, 8);

  Expression e4;

  e4.AddSummand(m1, Rational(7, 2), 7);
  e4.AddSummand(m2, Rational(9, 3), 8);

  Expression e5;

  e5.AddSummand(m1, Scalar(Rational(7, 2), 7));
  e5.AddSummand(m2, Scalar(Rational(9, 3), 8));

  EXPECT_EQ("1 eta^{ a b } s^{ y k l } v^{ n }\n+ 1 epsilon^{ a b y k } v^{ l } v^{ n }", e1.GetLatexString());
  EXPECT_EQ("7/2 eta^{ a b } s^{ y k l } v^{ n }\n+ 3*e_8 epsilon^{ a b y k } v^{ l } v^{ n }", e2.GetLatexString());
  EXPECT_EQ("1*e_7 eta^{ a b } s^{ y k l } v^{ n }\n+ 1*e_8 epsilon^{ a b y k } v^{ l } v^{ n }", e3.GetLatexString());
  EXPECT_EQ("7/2*e_7 eta^{ a b } s^{ y k l } v^{ n }\n+ 3*e_8 epsilon^{ a b y k } v^{ l } v^{ n }", e4.GetLatexString());
  EXPECT_EQ("7/2*e_7 eta^{ a b } s^{ y k l } v^{ n }\n+ 3*e_8 epsilon^{ a b y k } v^{ l } v^{ n }", e5.GetLatexString());
}

TEST(Expression, ApplyMonomialSymmetries) {
  Tensor eta (2, "eta");
  Tensor epsilon (4, "epsilon");
  const Tensor v (1, "v");
  const Tensor s (3, "s");

  eta.SetSymmetric();
  epsilon.SetAntisymmetric();

  const Indices i { 'b', 'y', 'a', 'k', 'l', 'n' };

  TensorMonomial tm1;
  TensorMonomial tm2;
  TensorMonomial tm3;

  tm1.AddFactorRight(eta);
  tm1.AddFactorRight(s);
  tm1.AddFactorRight(v);

  tm2.AddFactorRight(epsilon);
  tm2.AddFactorRight(v);
  tm2.AddFactorRight(v);

  tm3.AddFactorRight(v);
  tm3.AddFactorRight(eta);
  tm3.AddFactorRight(eta);
  tm3.AddFactorRight(v);

  const MonomialExpression m1 (tm1, i);
  const MonomialExpression m2 (tm2, i);
  const MonomialExpression m3 (tm3, i);

  Expression e;

  e.AddSummand(m1);
  e.AddSummand(m2, 5);
  e.AddSummand(m3, 5);

  EXPECT_EQ("1 eta^{ b y } s^{ a k l } v^{ n }\n+ 1*e_5 epsilon^{ b y a k } v^{ l } v^{ n }\n+ 1*e_5 v^{ b } eta^{ y a } eta^{ k l } v^{ n }", e.GetLatexString());

  e.ApplyMonomialSymmetries();

  EXPECT_EQ("1 eta^{ b y } s^{ a k l } v^{ n }\n- 1*e_5 epsilon^{ a b k y } v^{ l } v^{ n }\n+ 1*e_5 v^{ b } eta^{ a y } eta^{ k l } v^{ n }", e.GetLatexString());
}