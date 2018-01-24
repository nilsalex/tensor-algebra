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

TEST(Expression, SortSummands) {
  Tensor eta (2, "eta");
  eta.SetSymmetric();

  Tensor partial (1, "partial");
  Tensor xi      (1, "xi");

  TensorMonomial tm;

  tm.AddFactorRight(eta);
  tm.AddFactorRight(partial);
  tm.AddFactorRight(xi);

  Indices indices  {'e', 'g', 'h', 'f'};
  Indices indices2 {'e', 'g', 'f', 'h'};

  MonomialExpression m (tm, indices);

  Expression expr;
  expr.AddSummand(m);

  expr.ExchangeSymmetrise (indices, indices2, false);

  EXPECT_EQ("1/2 eta^{ e g } partial^{ h } xi^{ f }\n+ 1/2 eta^{ e g } partial^{ f } xi^{ h }", expr.GetLatexString());

  expr.SortSummands();

  EXPECT_EQ("1/2 eta^{ e g } partial^{ f } xi^{ h }\n+ 1/2 eta^{ e g } partial^{ h } xi^{ f }", expr.GetLatexString());
}

TEST(Expression, EliminateEpsilon) {
  Tensor epsilon (4, "epsilon");
  epsilon.SetAntisymmetric();

  Tensor eta (2, "eta");
  eta.SetSymmetric();

  Tensor partial (3, "partial");
  partial.SetSymmetric();

  Tensor xi (1, "xi");

  TensorMonomial tm;

  tm.AddFactorRight(epsilon);
  tm.AddFactorRight(eta);
  tm.AddFactorRight(eta);
  tm.AddFactorRight(partial);
  tm.AddFactorRight(xi);

  Indices indices { 'c', 'd', 'f', 'p', 'a', 'q', 'b', 'h', 'f', 'p', 'q', 'h' };

  MonomialExpression me (tm, indices);

  ScalarSum s;
  s.AddScalar(Scalar(2));
  s.AddScalar(Scalar(4));

  Expression expr;
  expr.AddSummand(me, s);

  EXPECT_EQ("(1*e_2 + 1*e_4) epsilon^{ c d f p } eta^{ a q } eta^{ b h } partial^{ f p q } xi^{ h }", expr.GetLatexString());

  expr.EliminateEpsilon();

  EXPECT_EQ("0", expr.GetLatexString());
}

TEST(Expression, EliminateEpsilonEpsilonI) {
  Tensor epsilon (4, "epsilon");
  Tensor epsilonI (4, "epsilonI");
  Tensor xi (1, "xi");

  epsilon.SetAntisymmetric();
  epsilonI.SetAntisymmetric();

  TensorMonomial tm;

  tm.AddFactorRight(epsilon);
  tm.AddFactorRight(epsilonI);
  tm.AddFactorRight(xi);

  Indices indices { 'a', 'b', 'c', 'd', 'p', 'q', 'r', 's', 'x' };

  MonomialExpression me (tm, indices);

  Expression expr;

  expr.AddSummand(me);

  EXPECT_EQ("1 epsilon^{ a b c d } epsilonI^{ p q r s } xi^{ x }", expr.GetLatexString());

  expr.EliminateEpsilonEpsilonI();

  EXPECT_EQ("- 1 delta^{ a p } delta^{ b q } delta^{ c r } delta^{ d s }\n+ 1 delta^{ a p } delta^{ b q } delta^{ c s } delta^{ d r }\n+ 1 delta^{ a p } delta^{ b r } delta^{ c q } delta^{ d s }\n- 1 delta^{ a p } delta^{ b r } delta^{ c s } delta^{ d q }\n- 1 delta^{ a p } delta^{ b s } delta^{ c q } delta^{ d r }\n+ 1 delta^{ a p } delta^{ b s } delta^{ c r } delta^{ d q }\n+ 1 delta^{ a q } delta^{ b p } delta^{ c r } delta^{ d s }\n- 1 delta^{ a q } delta^{ b p } delta^{ c s } delta^{ d r }\n- 1 delta^{ a q } delta^{ b r } delta^{ c p } delta^{ d s }\n+ 1 delta^{ a q } delta^{ b r } delta^{ c s } delta^{ d p }\n+ 1 delta^{ a q } delta^{ b s } delta^{ c p } delta^{ d r }\n- 1 delta^{ a q } delta^{ b s } delta^{ c r } delta^{ d p }\n- 1 delta^{ a r } delta^{ b p } delta^{ c q } delta^{ d s }\n+ 1 delta^{ a r } delta^{ b p } delta^{ c s } delta^{ d q }\n+ 1 delta^{ a r } delta^{ b q } delta^{ c p } delta^{ d s }\n- 1 delta^{ a r } delta^{ b q } delta^{ c s } delta^{ d p }\n- 1 delta^{ a r } delta^{ b s } delta^{ c p } delta^{ d q }\n+ 1 delta^{ a r } delta^{ b s } delta^{ c q } delta^{ d p }\n+ 1 delta^{ a s } delta^{ b p } delta^{ c q } delta^{ d r }\n- 1 delta^{ a s } delta^{ b p } delta^{ c r } delta^{ d q }\n- 1 delta^{ a s } delta^{ b q } delta^{ c p } delta^{ d r }\n+ 1 delta^{ a s } delta^{ b q } delta^{ c r } delta^{ d p }\n+ 1 delta^{ a s } delta^{ b r } delta^{ c p } delta^{ d q }\n- 1 delta^{ a s } delta^{ b r } delta^{ c q } delta^{ d p }", expr.GetLatexString());
}

TEST(Expression, EliminateDelta) {
  Tensor delta (2, "delta");
  delta.SetSymmetric();

  Tensor T (2, "T");

  TensorMonomial tm;

  tm.AddFactorRight(delta);
  tm.AddFactorRight(T);

  Indices indices  { 'a', 'a', 'x', 'k' };
  Indices indices2 { 'a', 'x', 'x', 'k' };

  MonomialExpression me  (tm, indices );
  MonomialExpression me2 (tm, indices2);

  Expression expr;
  expr.AddSummand(me);

  Expression expr2;
  expr2.AddSummand(me2);

  EXPECT_EQ("1 delta^{ a a } T^{ x k }", expr.GetLatexString());
  EXPECT_EQ("1 delta^{ a x } T^{ x k }", expr2.GetLatexString());

  expr.EliminateDelta();
  expr2.EliminateDelta();

  EXPECT_EQ("4 T^{ x k }", expr.GetLatexString());
  EXPECT_EQ("1 T^{ a k }", expr2.GetLatexString());
}
