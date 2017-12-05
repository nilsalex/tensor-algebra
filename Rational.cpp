#include "Rational.h"

Rational::Rational(Fraction const & fraction_set) : fraction(Cancel(fraction_set)){ }

void Rational::Cancel() {
  fraction = Cancel(fraction);
}

void Rational::DivideByTwo() {
  if (fraction.first % 2 == 0) {
    fraction.first = fraction.first / 2;
  } else {
    fraction.second = fraction.second * 2;
  }
}

void Rational::Negate() {
  fraction.first = -1 * fraction.first;
}

Fraction Rational::Cancel(Fraction const & fraction_set) {
  unsigned int gcd { GreatestCommonDivisor(fraction_set) };
  return std::make_pair(fraction_set.first / static_cast<int>(gcd), fraction_set.second / gcd);
}

unsigned int Rational::LeastCommonMultiple(Fraction const & fraction) {
  if (fraction.first == 0) {
    return 0;
  }
  unsigned int gcd = GreatestCommonDivisor(fraction);

  if (fraction.first < 0) {
    return (static_cast<unsigned int>(-fraction.first) * fraction.second) / gcd;
  } else {
    return (static_cast<unsigned int>(fraction.first) * fraction.second) / gcd;
  }
}

unsigned int Rational::GreatestCommonDivisor(Fraction const & fraction) {
  return (fraction.first < 0 ? GreatestCommonDivisor(std::make_pair(-fraction.first, fraction.second)) : (fraction.second == 0 ? fraction.first : GreatestCommonDivisor(std::make_pair(fraction.second, fraction.first % fraction.second))));
}

Rational Rational::AddOther (Rational const & other) const {
  return Rational(fraction.first * other.fraction.second + fraction.second * other.fraction.first, fraction.second * other.fraction.second);
}

Rational Rational::SubtractOther (Rational const & other) const {
  return Rational(fraction.first * other.fraction.second - fraction.second * other.fraction.first, fraction.second * other.fraction.second);
}

Rational Rational::DivideOther (Rational const & other) const {
  int numerator = fraction.first * static_cast<int>(other.fraction.second);
  int denominator = static_cast<int>(fraction.second) * other.fraction.first;

  if (numerator * denominator < 0) {
    return Rational(-1 * std::abs(numerator), static_cast<unsigned int>(std::abs(denominator)));
  } else {
    return Rational(std::abs(numerator), static_cast<unsigned int>(std::abs(denominator)));
  }
}

Rational Rational::MultiplyOther (Rational const & other) const {
  return Rational(fraction.first * other.fraction.first, fraction.second * other.fraction.second);
}

std::string const Rational::ToString(bool plus_sign) const {
  std::string ret_string = "";
  if (fraction.first == 0) {
    return "0";
  } else if (fraction.first > 0) {
    if (plus_sign) {
      ret_string += "+ ";
    }
  } else {
    ret_string += "- ";
  }

  ret_string += std::to_string(std::abs(fraction.first));

  if (fraction.second == 1) {
    return ret_string;
  }

  ret_string += "/";
  ret_string += std::to_string(fraction.second);

  return ret_string;
} 
