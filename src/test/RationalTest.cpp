#include "Rational.h"

#include "gtest/gtest.h"

TEST(Rational, DefaultConstructor) {
  const Rational r;

  EXPECT_EQ(0, r.get_fraction().first);
  EXPECT_EQ(1, r.get_fraction().second);
}

TEST(Rational, ConstructorFromFraction) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  EXPECT_EQ(0, r1.get_fraction().first);
  EXPECT_EQ(1, r1.get_fraction().second);

  EXPECT_EQ(-2, r2.get_fraction().first);
  EXPECT_EQ(5, r2.get_fraction().second);

  EXPECT_EQ(3, r3.get_fraction().first);
  EXPECT_EQ(1, r3.get_fraction().second);

  EXPECT_EQ(-533, r4.get_fraction().first);
  EXPECT_EQ(14, r4.get_fraction().second);
}

TEST(Rational, ConstructorFromNumDenom) {
  const int i1 = 0;
  const unsigned int j1 = 1;

  const int i2 = -2;
  const unsigned int j2 = 5;

  const int i3 = 99;
  const unsigned int j3 = 33;

  const int i4 = -1066;
  const unsigned int j4 = 28;

  const Rational r1(i1, j1);
  const Rational r2(i2, j2);
  const Rational r3(i3, j3);
  const Rational r4(i4, j4);

  EXPECT_EQ(0, r1.get_fraction().first);
  EXPECT_EQ(1, r1.get_fraction().second);

  EXPECT_EQ(-2, r2.get_fraction().first);
  EXPECT_EQ(5, r2.get_fraction().second);

  EXPECT_EQ(3, r3.get_fraction().first);
  EXPECT_EQ(1, r3.get_fraction().second);

  EXPECT_EQ(-533, r4.get_fraction().first);
  EXPECT_EQ(14, r4.get_fraction().second);
}

TEST(Rational, equals) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};

  const Fraction f3 {0, 789};
  const Fraction f4 {-50, 125};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  EXPECT_EQ(r1, r3);
  EXPECT_EQ(r2, r4);

  EXPECT_NE(r1, r2);
}

TEST(Rational, less_than) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Fraction f5 {0, 789};
  const Fraction f6 {-50, 125};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const Rational r5(f5);
  const Rational r6(f6);
  
  EXPECT_LT(r2, r1);
  EXPECT_LT(r1, r3);
  EXPECT_LT(r4, r1);
  EXPECT_LT(r2, r3);
  EXPECT_LT(r4, r2);
  EXPECT_LT(r4, r3);

  EXPECT_LE(r2, r1);
  EXPECT_LE(r1, r3);
  EXPECT_LE(r4, r1);
  EXPECT_LE(r2, r3);
  EXPECT_LE(r4, r2);
  EXPECT_LE(r4, r3);

  EXPECT_LE(r1, r5);
  EXPECT_LE(r2, r6);

  EXPECT_GT(r1, r2);
  EXPECT_GT(r3, r1);
  EXPECT_GT(r1, r4);
  EXPECT_GT(r3, r2);
  EXPECT_GT(r2, r4);
  EXPECT_GT(r3, r4);

  EXPECT_GE(r1, r2);
  EXPECT_GE(r3, r1);
  EXPECT_GE(r1, r4);
  EXPECT_GE(r3, r2);
  EXPECT_GE(r2, r4);
  EXPECT_GE(r3, r4);

  EXPECT_GE(r1, r5);
  EXPECT_GE(r2, r6);
}

TEST(Rational, IsZero) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  EXPECT_TRUE(r1.IsZero());
  EXPECT_FALSE(r1.IsPositive());
  EXPECT_FALSE(r1.IsNegative());

  EXPECT_FALSE(r2.IsZero());
  EXPECT_FALSE(r2.IsPositive());
  EXPECT_TRUE(r2.IsNegative());

  EXPECT_FALSE(r3.IsZero());
  EXPECT_TRUE(r3.IsPositive());
  EXPECT_FALSE(r3.IsNegative());

  EXPECT_FALSE(r4.IsZero());
  EXPECT_FALSE(r4.IsPositive());
  EXPECT_TRUE(r4.IsNegative());
}

TEST(Rational, GreatestCommonDivisor) {
  const Fraction f1 {0, 1};
  const Fraction f1s {1, 0};
  const Fraction f2 {-2, 5};
  const Fraction f2s {-5, 2};
  const Fraction f3 {99, 33};
  const Fraction f3s {33, 99};
  const Fraction f4 {-1066, 28};
  const Fraction f4s {-28, 1066};

  EXPECT_EQ(1, Rational::GreatestCommonDivisor(f1));
  EXPECT_EQ(1, Rational::GreatestCommonDivisor(f2));
  EXPECT_EQ(33, Rational::GreatestCommonDivisor(f3));
  EXPECT_EQ(2, Rational::GreatestCommonDivisor(f4));

  EXPECT_EQ(Rational::GreatestCommonDivisor(f1), Rational::GreatestCommonDivisor(f1s));
  EXPECT_EQ(Rational::GreatestCommonDivisor(f2), Rational::GreatestCommonDivisor(f2s));
  EXPECT_EQ(Rational::GreatestCommonDivisor(f3), Rational::GreatestCommonDivisor(f3s));
  EXPECT_EQ(Rational::GreatestCommonDivisor(f4), Rational::GreatestCommonDivisor(f4s));
}

TEST(Rational, LeastCommonMultiple) {
  const Fraction f1 {0, 1};
  const Fraction f1s {1, 0};
  const Fraction f2 {-2, 5};
  const Fraction f2s {-5, 2};
  const Fraction f3 {99, 33};
  const Fraction f3s {33, 99};
  const Fraction f4 {-1066, 28};
  const Fraction f4s {-28, 1066};

  EXPECT_EQ(0, Rational::LeastCommonMultiple(f1));
  EXPECT_EQ(10, Rational::LeastCommonMultiple(f2));
  EXPECT_EQ(99, Rational::LeastCommonMultiple(f3));
  EXPECT_EQ(14924, Rational::LeastCommonMultiple(f4));

  EXPECT_EQ(Rational::LeastCommonMultiple(f1), Rational::LeastCommonMultiple(f1s));
  EXPECT_EQ(Rational::LeastCommonMultiple(f2), Rational::LeastCommonMultiple(f2s));
  EXPECT_EQ(Rational::LeastCommonMultiple(f3), Rational::LeastCommonMultiple(f3s));
  EXPECT_EQ(Rational::LeastCommonMultiple(f4), Rational::LeastCommonMultiple(f4s));
}

TEST(Rational, Cancel) {
  const Fraction f1 {0, 1};
  const Fraction f1s {1, 0};
  const Fraction f2 {-2, 5};
  const Fraction f2s {-5, 2};
  const Fraction f3 {99, 33};
  const Fraction f3s {33, 99};
  const Fraction f4 {-1066, 28};
  const Fraction f4s {-28, 1066};

  const Fraction f1c = Rational::Cancel(f1);
  const Fraction f1sc = Rational::Cancel(f1s);
  const Fraction f2c = Rational::Cancel(f2);
  const Fraction f2sc = Rational::Cancel(f2s);
  const Fraction f3c = Rational::Cancel(f3);
  const Fraction f3sc = Rational::Cancel(f3s);
  const Fraction f4c = Rational::Cancel(f4);
  const Fraction f4sc = Rational::Cancel(f4s);

  EXPECT_EQ(f1, f1c);
  EXPECT_EQ(f1s, f1sc);
  
  EXPECT_EQ(f2, f2c);
  EXPECT_EQ(f2s, f2sc);

  EXPECT_EQ(Fraction(3, 1), f3c);
  EXPECT_EQ(Fraction(1, 3), f3sc);

  EXPECT_EQ(Fraction(-533, 14), f4c);
  EXPECT_EQ(Fraction(-14, 533), f4sc);
}

TEST(Rational, DivideByTwo) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  Rational r1(f1);
  Rational r2(f2);
  Rational r3(f3);
  Rational r4(f4);

  r1.DivideByTwo();
  r2.DivideByTwo();
  r3.DivideByTwo();
  r4.DivideByTwo();

  EXPECT_EQ(r1, Rational(0, 1));
  EXPECT_EQ(r2, Rational(-1, 5));
  EXPECT_EQ(r3, Rational(3, 2));
  EXPECT_EQ(r4, Rational(-533, 28));
}

TEST(Rational, Negate) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  Rational r1(f1);
  Rational r2(f2);
  Rational r3(f3);
  Rational r4(f4);

  r1.Negate();
  r2.Negate();
  r3.Negate();
  r4.Negate();

  EXPECT_EQ(r1, Rational(0, 1));
  EXPECT_EQ(r2, Rational(2, 5));
  EXPECT_EQ(r3, Rational(-3, 1));
  EXPECT_EQ(r4, Rational(533, 14));
}

TEST(Rational, AddOther) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const Rational r1_plus_r1(r1.AddOther(r1));
  const Rational r1_plus_r2(r1.AddOther(r2));
  const Rational r1_plus_r3(r1.AddOther(r3));
  const Rational r1_plus_r4(r1.AddOther(r4));

  const Rational r2_plus_r1(r2.AddOther(r1));
  const Rational r2_plus_r2(r2.AddOther(r2));
  const Rational r2_plus_r3(r2.AddOther(r3));
  const Rational r2_plus_r4(r2.AddOther(r4));

  const Rational r3_plus_r1(r3.AddOther(r1));
  const Rational r3_plus_r2(r3.AddOther(r2));
  const Rational r3_plus_r3(r3.AddOther(r3));
  const Rational r3_plus_r4(r3.AddOther(r4));

  const Rational r4_plus_r1(r4.AddOther(r1));
  const Rational r4_plus_r2(r4.AddOther(r2));
  const Rational r4_plus_r3(r4.AddOther(r3));
  const Rational r4_plus_r4(r4.AddOther(r4));

  EXPECT_EQ(r1_plus_r1, Rational(0, 1));
  EXPECT_EQ(r1_plus_r2, Rational(-2, 5));
  EXPECT_EQ(r1_plus_r3, Rational(3, 1));
  EXPECT_EQ(r1_plus_r4, Rational(-533, 14));
  
  EXPECT_EQ(r2_plus_r1, Rational(-2, 5));
  EXPECT_EQ(r2_plus_r2, Rational(-4, 5));
  EXPECT_EQ(r2_plus_r3, Rational(13, 5));
  EXPECT_EQ(r2_plus_r4, Rational(-2693, 70));
  
  EXPECT_EQ(r3_plus_r1, Rational(3, 1));
  EXPECT_EQ(r3_plus_r2, Rational(13, 5));
  EXPECT_EQ(r3_plus_r3, Rational(6, 1));
  EXPECT_EQ(r3_plus_r4, Rational(-491, 14));
  
  EXPECT_EQ(r4_plus_r1, Rational(-533, 14));
  EXPECT_EQ(r4_plus_r2, Rational(-2693, 70));
  EXPECT_EQ(r4_plus_r3, Rational(-491, 14));
  EXPECT_EQ(r4_plus_r4, Rational(-533, 7));
}

TEST(Rational, SubtractOther) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const Rational r1_minus_r1(r1.SubtractOther(r1));
  const Rational r1_minus_r2(r1.SubtractOther(r2));
  const Rational r1_minus_r3(r1.SubtractOther(r3));
  const Rational r1_minus_r4(r1.SubtractOther(r4));

  const Rational r2_minus_r1(r2.SubtractOther(r1));
  const Rational r2_minus_r2(r2.SubtractOther(r2));
  const Rational r2_minus_r3(r2.SubtractOther(r3));
  const Rational r2_minus_r4(r2.SubtractOther(r4));

  const Rational r3_minus_r1(r3.SubtractOther(r1));
  const Rational r3_minus_r2(r3.SubtractOther(r2));
  const Rational r3_minus_r3(r3.SubtractOther(r3));
  const Rational r3_minus_r4(r3.SubtractOther(r4));

  const Rational r4_minus_r1(r4.SubtractOther(r1));
  const Rational r4_minus_r2(r4.SubtractOther(r2));
  const Rational r4_minus_r3(r4.SubtractOther(r3));
  const Rational r4_minus_r4(r4.SubtractOther(r4));

  EXPECT_EQ(r1_minus_r1, Rational(0, 1));
  EXPECT_EQ(r1_minus_r2, Rational(2, 5));
  EXPECT_EQ(r1_minus_r3, Rational(-3, 1));
  EXPECT_EQ(r1_minus_r4, Rational(533, 14));
  
  EXPECT_EQ(r2_minus_r1, Rational(-2, 5));
  EXPECT_EQ(r2_minus_r2, Rational(0, 1));
  EXPECT_EQ(r2_minus_r3, Rational(-17, 5));
  EXPECT_EQ(r2_minus_r4, Rational(2637, 70));
  
  EXPECT_EQ(r3_minus_r1, Rational(3, 1));
  EXPECT_EQ(r3_minus_r2, Rational(17, 5));
  EXPECT_EQ(r3_minus_r3, Rational(0, 1));
  EXPECT_EQ(r3_minus_r4, Rational(575, 14));
  
  EXPECT_EQ(r4_minus_r1, Rational(-533, 14));
  EXPECT_EQ(r4_minus_r2, Rational(-2637, 70));
  EXPECT_EQ(r4_minus_r3, Rational(-575, 14));
  EXPECT_EQ(r4_minus_r4, Rational(0, 1));
}

TEST(Rational, MultiplyOther) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const Rational r1_times_r1(r1.MultiplyOther(r1));
  const Rational r1_times_r2(r1.MultiplyOther(r2));
  const Rational r1_times_r3(r1.MultiplyOther(r3));
  const Rational r1_times_r4(r1.MultiplyOther(r4));

  const Rational r2_times_r1(r2.MultiplyOther(r1));
  const Rational r2_times_r2(r2.MultiplyOther(r2));
  const Rational r2_times_r3(r2.MultiplyOther(r3));
  const Rational r2_times_r4(r2.MultiplyOther(r4));

  const Rational r3_times_r1(r3.MultiplyOther(r1));
  const Rational r3_times_r2(r3.MultiplyOther(r2));
  const Rational r3_times_r3(r3.MultiplyOther(r3));
  const Rational r3_times_r4(r3.MultiplyOther(r4));

  const Rational r4_times_r1(r4.MultiplyOther(r1));
  const Rational r4_times_r2(r4.MultiplyOther(r2));
  const Rational r4_times_r3(r4.MultiplyOther(r3));
  const Rational r4_times_r4(r4.MultiplyOther(r4));

  EXPECT_EQ(r1_times_r1, Rational(0, 1));
  EXPECT_EQ(r1_times_r2, Rational(0, 1));
  EXPECT_EQ(r1_times_r3, Rational(0, 1));
  EXPECT_EQ(r1_times_r4, Rational(0, 1));
  
  EXPECT_EQ(r2_times_r1, Rational(0, 1));
  EXPECT_EQ(r2_times_r2, Rational(4, 25));
  EXPECT_EQ(r2_times_r3, Rational(-6, 5));
  EXPECT_EQ(r2_times_r4, Rational(1066, 70));
  
  EXPECT_EQ(r3_times_r1, Rational(0, 1));
  EXPECT_EQ(r3_times_r2, Rational(-6, 5));
  EXPECT_EQ(r3_times_r3, Rational(9, 1));
  EXPECT_EQ(r3_times_r4, Rational(-1599, 14));
  
  EXPECT_EQ(r4_times_r1, Rational(0, 1));
  EXPECT_EQ(r4_times_r2, Rational(1066, 70));
  EXPECT_EQ(r4_times_r3, Rational(-1599, 14));
  EXPECT_EQ(r4_times_r4, Rational(533*533, 196));
}

TEST(Rational, DivideOther) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const Rational r1_div_r2(r1.DivideOther(r2));
  const Rational r1_div_r3(r1.DivideOther(r3));
  const Rational r1_div_r4(r1.DivideOther(r4));

  const Rational r2_div_r2(r2.DivideOther(r2));
  const Rational r2_div_r3(r2.DivideOther(r3));
  const Rational r2_div_r4(r2.DivideOther(r4));

  const Rational r3_div_r2(r3.DivideOther(r2));
  const Rational r3_div_r3(r3.DivideOther(r3));
  const Rational r3_div_r4(r3.DivideOther(r4));

  const Rational r4_div_r2(r4.DivideOther(r2));
  const Rational r4_div_r3(r4.DivideOther(r3));
  const Rational r4_div_r4(r4.DivideOther(r4));

  EXPECT_EQ(r1_div_r2, Rational(0, 1));
  EXPECT_EQ(r1_div_r3, Rational(0, 1));
  EXPECT_EQ(r1_div_r4, Rational(0, 1));
  
  EXPECT_EQ(r2_div_r2, Rational(1, 1));
  EXPECT_EQ(r2_div_r3, Rational(-2, 15));
  EXPECT_EQ(r2_div_r4, Rational(28, 2665));
  
  EXPECT_EQ(r3_div_r2, Rational(-15, 2));
  EXPECT_EQ(r3_div_r3, Rational(1, 1));
  EXPECT_EQ(r3_div_r4, Rational(-42, 533));
  
  EXPECT_EQ(r4_div_r2, Rational(2665, 28));
  EXPECT_EQ(r4_div_r3, Rational(-533, 42));
  EXPECT_EQ(r4_div_r4, Rational(1, 1));
}

TEST(Rational, ToString) {
  const Fraction f1 {0, 1};
  const Fraction f2 {-2, 5};
  const Fraction f3 {99, 33};
  const Fraction f4 {-1066, 28};

  const Rational r1(f1);
  const Rational r2(f2);
  const Rational r3(f3);
  const Rational r4(f4);

  const std::string s1 = r1.ToString(false);
  const std::string s2 = r2.ToString(false);
  const std::string s3 = r3.ToString(false);
  const std::string s4 = r4.ToString(false);

  const std::string s1_plus = r1.ToString(true);
  const std::string s2_plus = r2.ToString(true);
  const std::string s3_plus = r3.ToString(true);
  const std::string s4_plus = r4.ToString(true);

  EXPECT_EQ("0", s1);
  EXPECT_EQ("- 2/5", s2);
  EXPECT_EQ("3", s3);
  EXPECT_EQ("- 533/14", s4);

  EXPECT_EQ("+ 0", s1_plus);
  EXPECT_EQ("- 2/5", s2_plus);
  EXPECT_EQ("+ 3", s3_plus);
  EXPECT_EQ("- 533/14", s4_plus);
}
