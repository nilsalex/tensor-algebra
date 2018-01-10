#include "ScalarSum.h"

#include "gtest/gtest.h"

TEST(ScalarSum, DefaultConstructor) {
  const ScalarSum ss;

  EXPECT_EQ("0", ss.ToString());

  EXPECT_TRUE(ss.IsZero());
}

TEST(ScalarSum, ConstructorFromScalar) {
  const Scalar s (Rational(9, 3), 5);
  const ScalarSum ss(s);

  EXPECT_EQ("3*e_5", ss.ToString());
}

TEST(ScalarSum, CopyConstructor) {
  const Scalar s (Rational(27, 9), 4);
  const ScalarSum ss(s);
  const ScalarSum ss2(s);

  EXPECT_EQ(ss.ToString(), ss2.ToString());
  EXPECT_EQ("3*e_4", ss2.ToString());
}

TEST(ScalarSum, IsZero) {
  const ScalarSum ss;
  const Scalar s1 (Rational(-2, 5), 2);
  const Scalar s2 (Rational(3, 4));
  const Scalar s3;
  ScalarSum ss2 (s1);
  ss2.AddScalar(s2);
  ss2.AddScalar(s3);

  EXPECT_FALSE(ss2.IsZero());

  ss2.MultiplyCoefficient(Rational(0, 1));

  EXPECT_TRUE(ss.IsZero());
  EXPECT_TRUE(ss2.IsZero());
}

TEST(ScalarSum, ToString) {
  const ScalarSum ss1;
  const ScalarSum ss2(Scalar(Rational(-12, 4), 8));
  ScalarSum ss3;

  ss3.AddScalar(Scalar(Rational(1, 1)));
  ss3.AddScalar(Scalar(12));
  ss3.AddScalar(Scalar(Rational(7, 9), 2));

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("- 3*e_8", ss2.ToString());
  EXPECT_EQ("(1 + 1*e_12 + 7/9*e_2)", ss3.ToString());

  EXPECT_EQ("0", ss1.ToString("f^"));
  EXPECT_EQ("- 3*f^8", ss2.ToString("f^"));
  EXPECT_EQ("(1 + 1*f^12 + 7/9*f^2)", ss3.ToString("f^"));

  EXPECT_EQ("+ 0", ss1.ToString("x#", true));
  EXPECT_EQ("- 3*x#8", ss2.ToString("x#", true));
  EXPECT_EQ("+ (1 + 1*x#12 + 7/9*x#2)", ss3.ToString("x#", true));
}

TEST(ScalarSum, Negate) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));

  ss1.Negate();
  ss2.Negate();

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(- 1 - 1*e_12 - 7/9*e_2)", ss2.ToString());
}

TEST(ScalarSum, DivideByTwo) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));

  ss1.DivideByTwo();
  ss2.DivideByTwo();

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(1/2 + 1/2*e_12 + 7/18*e_2)", ss2.ToString());
}

TEST(ScalarSum, DivideCoefficient) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));

  ss1.DivideCoefficient(Rational(99, 1));
  ss2.DivideCoefficient(Rational(-2, 5));

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(- 5/2 - 5/2*e_12 - 35/18*e_2)", ss2.ToString());
}

TEST(ScalarSum, MultiplyCoefficient) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));

  ss1.MultiplyCoefficient(Rational(99, 1));
  ss2.MultiplyCoefficient(Rational(-2, 5));

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(- 2/5 - 2/5*e_12 - 14/45*e_2)", ss2.ToString());
}

TEST(ScalarSum, AddScalar) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss1.AddScalar((Scalar(Rational(0, 1))));

  ss2.AddScalar((Scalar(Rational(0, 1))));

  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));

  ss2.AddScalar((Scalar(Rational(0, 1))));

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(1 + 1*e_12 + 7/9*e_2)", ss2.ToString());
}

TEST(ScalarSum, MergeWithOther) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss1.AddScalar(Scalar(Rational(1, 5)));
  ss2.AddScalar(Scalar(Rational(2, 3), 9));
  ss2.AddScalar(Scalar(Rational(-2, 3), 90));

  ss1.MergeWithOther(ss2);

  EXPECT_EQ("(1/5 + 2/3*e_9 - 2/3*e_90)", ss1.ToString());
}

TEST(ScalarSum, Sort) {
  ScalarSum ss1;
  ScalarSum ss2;

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss1.Sort();
  ss2.Sort();

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(1 + 7/10*e_2 + 7/9*e_2 + 1*e_12)", ss2.ToString());
}

TEST(ScalarSum, Collect) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(1,1)));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss3.AddScalar(Scalar(Rational(2, 1), 3));
  ss3.AddScalar(Scalar(Rational(4, 1), 3));
  ss3.AddScalar(Scalar(Rational(-6, 1), 3));

  ss1.Collect();
  ss2.Collect();
  ss3.Collect();

  EXPECT_EQ("0", ss1.ToString());
  EXPECT_EQ("(1*e_12 + 1 + 133/90*e_2)", ss2.ToString());
  EXPECT_EQ("0", ss3.ToString());

}

TEST(ScalarSum, MultiplyOther) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;

  ss1.AddScalar(Scalar(Rational(5, 3)));
  ss1.AddScalar(Scalar(Rational(5, 2), 3));

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  EXPECT_EQ("(5/3 + 5/2*e_3)", ss1.ToString());
  EXPECT_EQ("(1*e_12 + 7/10*e_2)", ss2.ToString());
  EXPECT_EQ("0", ss3.ToString());

  ScalarSum ss4 = ss1.MultiplyOther(ss2);
  ScalarSum ss5 = ss1.MultiplyOther(ss3);
  ScalarSum ss6 = ss3.MultiplyOther(ss2);
  ScalarSum ss7 = ss3.MultiplyOther(ss3);

  EXPECT_EQ("(7/6*e_2 + 7/4*e_2*e_3 + 5/2*e_3*e_12 + 5/3*e_12)", ss4.ToString());
  EXPECT_EQ("0", ss5.ToString());
  EXPECT_EQ("0", ss6.ToString());
  EXPECT_EQ("0", ss7.ToString());

}

TEST(ScalarSum, CoefficientSet) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;
  
  ss1.AddScalar(Scalar(1));

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss3.AddScalar(Scalar(Rational(2, 1), 3));
  ss3.AddScalar(Scalar(Rational(4, 1), 3));
  ss3.AddScalar(Scalar(Rational(-6, 1), 3));

  std::set<size_t> set1 {1};
  std::set<size_t> set2 {2, 12};
  std::set<size_t> set3 {3};

  EXPECT_EQ(set1, ss1.CoefficientSet());
  EXPECT_EQ(set2, ss2.CoefficientSet());
  EXPECT_EQ(set3, ss3.CoefficientSet());
}

TEST(ScalarSum, CoefficientVector) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;
  
  ss1.AddScalar(Scalar(1));

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss3.AddScalar(Scalar(Rational(2, 1), 3));
  ss3.AddScalar(Scalar(Rational(4, 1), 3));
  ss3.AddScalar(Scalar(Rational(-6, 1), 3));

  const std::map<size_t, size_t> map1 {{1, 0}, {3, 1}, {99, 2}};
  const std::map<size_t, size_t> map2 {{1, 0}, {2, 1}, {3, 2}, {9, 3}, {12, 4}, {9989, 5}};
  const std::map<size_t, size_t> map3 {{3, 0}};

  const std::vector<Rational> vec1 {Rational(1, 1), Rational(0, 1), Rational(0, 1)};
  const std::vector<Rational> vec2 {Rational(0, 1), Rational(133, 90), Rational(0, 1), Rational(0, 1), Rational(1, 1), Rational(0, 1)};
  const std::vector<Rational> vec3 {Rational(0, 1)};

  EXPECT_EQ(vec1, ss1.CoefficientVector(map1)); 
  EXPECT_EQ(vec2, ss2.CoefficientVector(map2)); 
  EXPECT_EQ(vec3, ss3.CoefficientVector(map3)); 
}

TEST(ScalarSum, equals) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;
  
  ss1.AddScalar(Scalar(Rational(0, 1)));

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss3.AddScalar(Scalar(Rational(2, 1), 3));
  ss3.AddScalar(Scalar(Rational(4, 1), 3));
  ss3.AddScalar(Scalar(Rational(-6, 1), 3));

  EXPECT_EQ(ss1, ss1);
  EXPECT_NE(ss1, ss2);
  EXPECT_EQ(ss1, ss3);
  EXPECT_NE(ss2, ss1);
  EXPECT_EQ(ss2, ss2);
  EXPECT_NE(ss2, ss3);
  EXPECT_EQ(ss3, ss1);
  EXPECT_NE(ss3, ss2);
  EXPECT_EQ(ss3, ss3);

  ss3.Collect();

  EXPECT_EQ(ss1, ss3);
  EXPECT_NE(ss2, ss3);
  EXPECT_EQ(ss3, ss1);
  EXPECT_NE(ss3, ss2);
  EXPECT_EQ(ss3, ss3);
}

TEST(ScalarSum, lessThan) {
  ScalarSum ss1;
  ScalarSum ss2;
  ScalarSum ss3;
  
  ss1.AddScalar(Scalar(Rational(0, 1)));

  ss2.AddScalar(Scalar(12));
  ss2.AddScalar(Scalar(Rational(7, 9), 2));
  ss2.AddScalar(Scalar(Rational(7, 10), 2));

  ss3.AddScalar(Scalar(Rational(2, 1), 3));
  ss3.AddScalar(Scalar(Rational(4, 1), 3));
  ss3.AddScalar(Scalar(Rational(-6, 1), 3));

  EXPECT_FALSE(ss1 < ss1);
  EXPECT_TRUE(ss1 < ss2);
  EXPECT_FALSE(ss1 < ss3);
  EXPECT_FALSE(ss2 < ss1);
  EXPECT_FALSE(ss2 < ss2);
  EXPECT_FALSE(ss2 < ss3);
  EXPECT_FALSE(ss3 < ss1);
  EXPECT_TRUE(ss3 < ss2);
  EXPECT_FALSE(ss3 < ss3);

  ss3.Collect();

  EXPECT_FALSE(ss1 < ss3);
  EXPECT_FALSE(ss2 < ss3);
  EXPECT_FALSE(ss3 < ss1);
  EXPECT_TRUE(ss3 < ss2);
  EXPECT_FALSE(ss3 < ss3);
}
