#pragma once

#include <utility>
#include <string>

typedef std::pair<int, unsigned int> Fraction;

class Rational {
 private:
  Fraction fraction;

 public:
  Rational() : Rational(std::make_pair(0, 1)) { };
  Rational(Fraction const & fraction);
  Rational(int numerator, unsigned int denominator) : Rational(std::make_pair(numerator, denominator)) { };

  Fraction get_fraction() const { return fraction; }

  Rational AddOther (Rational const & other) const;
  Rational SubtractOther (Rational const & other) const;
  Rational DivideOther (Rational const & other) const;
  Rational MultiplyOther (Rational const & other) const;

  void Cancel();
  void DivideByTwo();
  void Negate();

  bool IsPositive() const { return fraction.first > 0; };
  bool IsNegative() const { return fraction.first < 0; };
  bool IsZero()     const { return fraction.first == 0; };

  static unsigned int GreatestCommonDivisor(Fraction const & fraction);
  static unsigned int LeastCommonMultiple(Fraction const & fraction);
  static Fraction Cancel(Fraction const & fraction);

  std::string const ToString(bool plus_sign = false) const;

  bool operator== (Rational const & other) const { return this->SubtractOther(other).fraction.first == 0; };
  bool operator!= (Rational const & other) const { return !(*this == other); };
  bool operator< (Rational const & other) const { return this->SubtractOther(other).fraction.first < 0; };
  bool operator> (Rational const & other) const { return other < *this; };
  bool operator<= (Rational const & other) const { return !(*this > other); };
  bool operator>= (Rational const & other) const { return !(*this < other); };

};
