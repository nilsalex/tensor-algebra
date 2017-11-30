#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "Rational.h"

class Scalar {
 private:
  std::vector<std::string> variables;
  Rational coefficient;

 public:
  Scalar() : variables(), coefficient(1,1) { };
  Scalar(Scalar const & other) : variables(other.variables), coefficient(other.coefficient) { };

  Scalar(Rational const & coefficient_set) : variables(), coefficient(coefficient_set) { };
  Scalar(std::string variable_set) : variables(), coefficient(1,1) { variables.push_back(variable_set); };
  Scalar(Rational coefficient_set, std::string variable_set) : variables(), coefficient(coefficient_set) { variables.push_back(variable_set); };


  void AddOther(Scalar const & other) { coefficient = coefficient.AddOther(other.coefficient); };
  bool CompareVariables(Scalar const & other) const { return variables == other.variables; };
  bool IsZero() const { return coefficient.IsZero(); };
  void Sort() { std::sort(variables.begin(), variables.end()); };

  std::string ToString(bool plus_sign = false) const;

  void DivideByTwo() { coefficient.DivideByTwo(); };
  void Negate() { coefficient.Negate(); };

  bool operator== (Scalar const & other) const;
  bool operator< (Scalar const & other) const;

  ~Scalar() = default;
};

