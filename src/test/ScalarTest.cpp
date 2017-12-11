#include "Scalar.h"

#include "gtest/gtest.h"

TEST(Scalar, DefaultConstructor) {
  const Scalar s;

  EXPECT_EQ(0, s.Order());
  EXPECT_EQ(Rational(1,1), s.get_coefficient());

  EXPECT_EQ("1", s.ToString());
}

TEST(Scalar, CopyConstructor) {
  const Scalar s(Rational(-7, 88), 9);
  const Scalar s2(s);

  EXPECT_EQ(s, s2);
}

TEST(Scalar, ConstructorFromCoefficient) {
  const Rational r (-213, 15);
  const Scalar s(r);
  const Scalar t((Rational()));

  EXPECT_EQ(0, s.Order());
  EXPECT_EQ(r, s.get_coefficient());
  EXPECT_EQ("- 71/5", s.ToString());

  EXPECT_EQ(0, t.Order());
  EXPECT_EQ(Rational(0, 1), t.get_coefficient());
  EXPECT_EQ("0", t.ToString());
}

TEST(Scalar, ConstructorFromVariable) {
  const Scalar s1(7);
  const Scalar s2(11);
  const Scalar s3(5);

  EXPECT_EQ(1, s1.Order());
  EXPECT_EQ(1, s2.Order());
  EXPECT_EQ(1, s3.Order());

  EXPECT_EQ(Rational(1,1), s1.get_coefficient());
  EXPECT_EQ(Rational(1,1), s2.get_coefficient());
  EXPECT_EQ(Rational(1,1), s3.get_coefficient());

  EXPECT_EQ(7, s1.VariableNumber());
  EXPECT_EQ(11, s2.VariableNumber());
  EXPECT_EQ(5, s3.VariableNumber());

  EXPECT_EQ("1*e_7", s1.ToString());
  EXPECT_EQ("1*e_11", s2.ToString());
  EXPECT_EQ("1*e_5", s3.ToString());
}

TEST(Scalar, ConstructorFromCoefficientVariable) {
  const Rational r (-87, 19);
  const Scalar s (r, 9);

  EXPECT_EQ(1, s.Order());
  EXPECT_EQ(Rational(-87, 19), s.get_coefficient());
  EXPECT_EQ(9, s.VariableNumber());

  EXPECT_EQ("- 87/19*e_9", s.ToString());
}

TEST(Scalar, GetCoefficient) {
  const Rational r (21, 7);
  const Scalar s (r, 11);

  EXPECT_EQ(r, s.get_coefficient());
}

TEST(Scalar, Order) {
  const Rational r (18, 9);
  Scalar s (r, 2);

  EXPECT_EQ(1, s.Order());

  s.MultiplyCoefficient(Rational(0, 1));

  EXPECT_EQ(0, s.Order());
}

TEST(Scalar, VariableNumber) {
  const Scalar s1(7);
  const Scalar s2(Rational(8, 4), 11);

  EXPECT_EQ(7, s1.VariableNumber());
  EXPECT_EQ(11, s2.VariableNumber());
}

TEST(Scalar, AddOther) {
  const Scalar s1(11);
  Scalar s2(Rational(-14, 35), 11);
  s2.AddOther(s1);

  const Scalar s3(Rational(7, 2));
  Scalar s4(Rational(7, 3));
  s4.AddOther(s3);

  EXPECT_EQ(Rational(3, 5), s2.get_coefficient());
  EXPECT_EQ(Rational(35, 6), s4.get_coefficient());
}

TEST(Scalar, CompareVariables) {
  const Scalar s1(Rational(2, 3), 18);
  const Scalar s2(7);
  const Scalar s3(Rational(5, 9));
  const Scalar s4;

  EXPECT_TRUE(s1.CompareVariables(s1));
  EXPECT_FALSE(s1.CompareVariables(s2));
  EXPECT_FALSE(s1.CompareVariables(s3));
  EXPECT_FALSE(s1.CompareVariables(s4));

  EXPECT_FALSE(s2.CompareVariables(s1));
  EXPECT_TRUE(s2.CompareVariables(s2));
  EXPECT_FALSE(s2.CompareVariables(s3));
  EXPECT_FALSE(s2.CompareVariables(s4));

  EXPECT_FALSE(s3.CompareVariables(s1));
  EXPECT_FALSE(s3.CompareVariables(s2));
  EXPECT_TRUE(s3.CompareVariables(s3));
  EXPECT_TRUE(s3.CompareVariables(s4));

  EXPECT_FALSE(s4.CompareVariables(s1));
  EXPECT_FALSE(s4.CompareVariables(s2));
  EXPECT_TRUE(s4.CompareVariables(s3));
  EXPECT_TRUE(s4.CompareVariables(s4));
}

TEST(Scalar, IsZero) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  const Rational r (0, 1);

  EXPECT_FALSE(s1.IsZero());
  EXPECT_FALSE(s2.IsZero());
  EXPECT_FALSE(s3.IsZero());
  EXPECT_FALSE(s4.IsZero());
  EXPECT_TRUE(s5.IsZero());
  EXPECT_TRUE(s6.IsZero());

  s1.MultiplyCoefficient(r);
  s2.MultiplyCoefficient(r);
  s3.MultiplyCoefficient(r);
  s4.MultiplyCoefficient(r);
  s5.MultiplyCoefficient(r);
  s6.MultiplyCoefficient(r);

  EXPECT_TRUE(s1.IsZero());
  EXPECT_TRUE(s2.IsZero());
  EXPECT_TRUE(s3.IsZero());
  EXPECT_TRUE(s4.IsZero());
  EXPECT_TRUE(s5.IsZero());
  EXPECT_TRUE(s6.IsZero());
}

TEST(Scalar, MultiplyCoefficient) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  const Rational r1 (0, 1);
  const Rational r2 (-8, 2);
  const Rational r3 (99, 103);

  s1.MultiplyCoefficient(r2);
  s2.MultiplyCoefficient(r2);
  s3.MultiplyCoefficient(r2);
  s4.MultiplyCoefficient(r2);
  s5.MultiplyCoefficient(r2);
  s6.MultiplyCoefficient(r2);

  EXPECT_EQ(Rational(-4, 1), s1.get_coefficient());
  EXPECT_EQ(Rational(-4, 1), s2.get_coefficient());
  EXPECT_EQ(Rational(-10, 1), s3.get_coefficient());
  EXPECT_EQ(Rational(-2, 1), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());

  s1.MultiplyCoefficient(r3);
  s2.MultiplyCoefficient(r3);
  s3.MultiplyCoefficient(r3);
  s4.MultiplyCoefficient(r3);
  s5.MultiplyCoefficient(r3);
  s6.MultiplyCoefficient(r3);

  EXPECT_EQ(Rational(-396, 103), s1.get_coefficient());
  EXPECT_EQ(Rational(-396, 103), s2.get_coefficient());
  EXPECT_EQ(Rational(-990, 103), s3.get_coefficient());
  EXPECT_EQ(Rational(-198, 103), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());

  s1.MultiplyCoefficient(r1);
  s2.MultiplyCoefficient(r1);
  s3.MultiplyCoefficient(r1);
  s4.MultiplyCoefficient(r1);
  s5.MultiplyCoefficient(r1);
  s6.MultiplyCoefficient(r1);

  EXPECT_EQ(Rational(0, 1), s1.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s2.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s3.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());
}

TEST(Scalar, DivideCoefficient) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  const Rational r1 (-8, 2);
  const Rational r2 (99, 103);

  s1.DivideCoefficient(r1);
  s2.DivideCoefficient(r1);
  s3.DivideCoefficient(r1);
  s4.DivideCoefficient(r1);
  s5.DivideCoefficient(r1);
  s6.DivideCoefficient(r1);

  EXPECT_EQ(Rational(-1, 4), s1.get_coefficient());
  EXPECT_EQ(Rational(-1, 4), s2.get_coefficient());
  EXPECT_EQ(Rational(-5, 8), s3.get_coefficient());
  EXPECT_EQ(Rational(-1, 8), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());

  s1.DivideCoefficient(r2);
  s2.DivideCoefficient(r2);
  s3.DivideCoefficient(r2);
  s4.DivideCoefficient(r2);
  s5.DivideCoefficient(r2);
  s6.DivideCoefficient(r2);

  EXPECT_EQ(Rational(-103, 396), s1.get_coefficient());
  EXPECT_EQ(Rational(-103, 396), s2.get_coefficient());
  EXPECT_EQ(Rational(-515, 792), s3.get_coefficient());
  EXPECT_EQ(Rational(-103, 792), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());
}

TEST(Scalar, DivideByTwo) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  s1.DivideByTwo();
  s2.DivideByTwo();
  s3.DivideByTwo();
  s4.DivideByTwo();
  s5.DivideByTwo();
  s6.DivideByTwo();

  EXPECT_EQ(Rational(1, 2), s1.get_coefficient());
  EXPECT_EQ(Rational(1, 2), s2.get_coefficient());
  EXPECT_EQ(Rational(5, 4), s3.get_coefficient());
  EXPECT_EQ(Rational(1, 4), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());
}

TEST(Scalar, Negate) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  s1.Negate();
  s2.Negate();
  s3.Negate();
  s4.Negate();
  s5.Negate();
  s6.Negate();

  EXPECT_EQ(Rational(-1, 1), s1.get_coefficient());
  EXPECT_EQ(Rational(-1, 1), s2.get_coefficient());
  EXPECT_EQ(Rational(-5, 2), s3.get_coefficient());
  EXPECT_EQ(Rational(-1, 2), s4.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s5.get_coefficient());
  EXPECT_EQ(Rational(0, 1), s6.get_coefficient());
}

TEST(Scalar, equals) {
  Scalar s1;
  Scalar s2(7);
  Scalar s3(Rational(5, 2), 2);
  Scalar s4(Rational(1, 2));
  Scalar s5(Rational(0, 3), 3);
  Scalar s6(Rational(0, 3));

  EXPECT_TRUE(s1 == s1);
  EXPECT_FALSE(s1 == s2);
  EXPECT_FALSE(s1 == s3);
  EXPECT_FALSE(s1 == s4);
  EXPECT_FALSE(s1 == s5);
  EXPECT_FALSE(s1 == s6);

  EXPECT_FALSE(s2 == s1);
  EXPECT_TRUE(s2 == s2);
  EXPECT_FALSE(s2 == s3);
  EXPECT_FALSE(s2 == s4);
  EXPECT_FALSE(s2 == s5);
  EXPECT_FALSE(s2 == s6);

  EXPECT_FALSE(s3 == s1);
  EXPECT_FALSE(s3 == s2);
  EXPECT_TRUE(s3 == s3);
  EXPECT_FALSE(s3 == s4);
  EXPECT_FALSE(s3 == s5);
  EXPECT_FALSE(s3 == s6);

  EXPECT_FALSE(s4 == s1);
  EXPECT_FALSE(s4 == s2);
  EXPECT_FALSE(s4 == s3);
  EXPECT_TRUE(s4 == s4);
  EXPECT_FALSE(s4 == s5);
  EXPECT_FALSE(s4 == s6);

  EXPECT_FALSE(s5 == s1);
  EXPECT_FALSE(s5 == s2);
  EXPECT_FALSE(s5 == s3);
  EXPECT_FALSE(s5 == s4);
  EXPECT_TRUE(s5 == s5);
  EXPECT_TRUE(s5 == s6);

  EXPECT_FALSE(s6 == s1);
  EXPECT_FALSE(s6 == s2);
  EXPECT_FALSE(s6 == s3);
  EXPECT_FALSE(s6 == s4);
  EXPECT_TRUE(s6 == s5);
  EXPECT_TRUE(s6 == s6);
}

TEST(Scalar, lessThan) {
  const Scalar s1;  
  const Scalar s2(Rational(7, 2));  
  const Scalar s3(5);  
  const Scalar s4(Rational(-2, 1), 5);  
  const Scalar s5(Rational(-3, 1), 6);  

  EXPECT_FALSE(s1 < s1);
  EXPECT_TRUE(s1 < s2);
  EXPECT_TRUE(s1 < s3);
  EXPECT_TRUE(s1 < s4);
  EXPECT_TRUE(s1 < s5);

  EXPECT_FALSE(s2 < s1);
  EXPECT_FALSE(s2 < s2);
  EXPECT_TRUE(s2 < s3);
  EXPECT_TRUE(s2 < s4);
  EXPECT_TRUE(s2 < s5);

  EXPECT_FALSE(s3 < s1);
  EXPECT_FALSE(s3 < s2);
  EXPECT_FALSE(s3 < s3);
  EXPECT_FALSE(s3 < s4);
  EXPECT_TRUE(s3 < s5);

  EXPECT_FALSE(s4 < s1);
  EXPECT_FALSE(s4 < s2);
  EXPECT_TRUE(s4 < s3);
  EXPECT_FALSE(s4 < s4);
  EXPECT_TRUE(s4 < s5);

  EXPECT_FALSE(s5 < s1);
  EXPECT_FALSE(s5 < s2);
  EXPECT_FALSE(s5 < s3);
  EXPECT_FALSE(s5 < s4);
  EXPECT_FALSE(s5 < s5);
}

TEST(Scalar, ToString) {
  const Scalar s1;
  const Scalar s2(7);
  const Scalar s3(Rational(2, 7));
  const Scalar s4(Rational(-2, 3));
  const Scalar s5(Rational(3, 19), 5);
  const Scalar s6(Rational(-22, 17), 6);

  EXPECT_EQ("1", s1.ToString());
  EXPECT_EQ("1", s1.ToString("e_"));
  EXPECT_EQ("1", s1.ToString("e-", false));
  EXPECT_EQ("+ 1", s1.ToString("e-", true));

  EXPECT_EQ("1*e_7", s2.ToString());
  EXPECT_EQ("1*f_7", s2.ToString("f_"));
  EXPECT_EQ("1*y^7", s2.ToString("y^", false));
  EXPECT_EQ("+ 1*z#7", s2.ToString("z#", true));

  EXPECT_EQ("2/7", s3.ToString());
  EXPECT_EQ("2/7", s3.ToString("h/"));
  EXPECT_EQ("2/7", s3.ToString("gehase", false));
  EXPECT_EQ("+ 2/7", s3.ToString("99??ß", true));

  EXPECT_EQ("- 2/3", s4.ToString());
  EXPECT_EQ("- 2/3", s4.ToString("fdsaf"));
  EXPECT_EQ("- 2/3", s4.ToString("b<", false));
  EXPECT_EQ("- 2/3", s4.ToString("99", true));

  EXPECT_EQ("3/19*e_5", s5.ToString());
  EXPECT_EQ("3/19*==5", s5.ToString("=="));
  EXPECT_EQ("3/19*u[5", s5.ToString("u[", false));
  EXPECT_EQ("+ 3/19*zz5", s5.ToString("zz", true));

  EXPECT_EQ("- 22/17*e_6", s6.ToString());
  EXPECT_EQ("- 22/17*9_6", s6.ToString("9_"));
  EXPECT_EQ("- 22/17*´´6", s6.ToString("´´", false));
  EXPECT_EQ("- 22/17*'#6", s6.ToString("'#", true));
}
