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
  Scalar() : variables(), coefficient(1,1) { };
  Scalar(Scalar const & other) : variables(other.variables), coefficient(other.coefficient) { };

  Scalar(Rational const & coefficient_set) : variables(), coefficient(coefficient_set) { };
  Scalar(size_t variable_set) : variables(), coefficient(1,1) { variables.insert(variable_set); };
  Scalar(Rational coefficient_set, size_t variable_set) : variables(), coefficient(coefficient_set) { variables.insert(variable_set); };

  Rational get_coefficient() const { return coefficient; };

  size_t Order() const { return (IsZero() ? 0 : variables.size()); };

  size_t VariableNumber() const { assert(variables.size() == 1); return *(variables.begin()); };

  void AddOther(Scalar const & other) { assert(CompareVariables(other)); coefficient = coefficient.AddOther(other.coefficient); };
  bool CompareVariables(Scalar const & other) const { return variables == other.variables; };
  bool IsZero() const { return coefficient.IsZero(); };

  void MultiplyCoefficient(Rational const & coeff) { coefficient = coefficient.MultiplyOther(coeff); };
  void DivideCoefficient(Rational const & coeff) { coefficient = coefficient.DivideOther(coeff); };

  std::string ToString(std::string base_name = "e_", bool plus_sign = false) const;

  void DivideByTwo() { coefficient.DivideByTwo(); };
  void Negate() { coefficient.Negate(); };

  bool operator== (Scalar const & other) const;
  bool operator< (Scalar const & other) const;

  ~Scalar() = default;
};

