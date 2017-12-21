#pragma once

#include <algorithm>
#include <cassert>
#include <set>
#include <string>

#include "Rational.h"

class Scalar {
 private:
  std::multiset<size_t> variables;
  Rational coefficient;

 public:
  Scalar();
  Scalar(Scalar const & other);

  Scalar(Rational const & coefficient_set);
  Scalar(size_t variable_set);
  Scalar(Rational coefficient_set, size_t variable_set);

  Rational get_coefficient() const;

  size_t Order() const;

  size_t VariableNumber() const;

  void AddOther(Scalar const & other);
  bool CompareVariables(Scalar const & other) const;
  bool IsZero() const;

  void MultiplyCoefficient(Rational const & coeff);
  void DivideCoefficient(Rational const & coeff);

  std::string ToString(std::string base_name = "e_", bool plus_sign = false) const;

  void DivideByTwo();
  void Negate();

  bool operator== (Scalar const & other) const;
  bool operator< (Scalar const & other) const;

  ~Scalar() = default;
};

