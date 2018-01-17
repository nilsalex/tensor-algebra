#pragma once

#include <utility>
#include <string>

#include <gmpxx.h>

#include <boost/serialization/utility.hpp>

typedef std::pair<int, unsigned int> Fraction;

class Rational {
 private:
  Fraction fraction;

  friend class boost::serialization::access;

  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  Rational();
  Rational(Fraction const & fraction);
  Rational(int numerator, unsigned int denominator);
  Rational(mpq_class q);

  Fraction get_fraction() const;

  Rational AddOther (Rational const & other) const;
  Rational SubtractOther (Rational const & other) const;
  Rational DivideOther (Rational const & other) const;
  Rational MultiplyOther (Rational const & other) const;

  void DivideByTwo();
  void Negate();

  bool IsPositive() const;
  bool IsNegative() const;
  bool IsZero() const;

  static unsigned int GreatestCommonDivisor(Fraction const & fraction);
  static unsigned int LeastCommonMultiple(Fraction const & fraction);
  static Fraction Cancel(Fraction const & fraction);

  std::string const ToString(bool plus_sign = false) const;

  bool operator== (Rational const & other) const;
  bool operator!= (Rational const & other) const;
  bool operator< (Rational const & other) const;
  bool operator> (Rational const & other) const;
  bool operator<= (Rational const & other) const;
  bool operator>= (Rational const & other) const;

};
