#pragma once

#include <algorithm>
#include <cassert>
#include <set>
#include <string>

#include <boost/serialization/set.hpp>

#include "Rational.h"

class Scalar {
 private:
  std::multiset<size_t> variables;
  Rational coefficient;

  friend class boost::serialization::access;

  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  Scalar();
  Scalar(Scalar const & other);

  Scalar(Rational const & coefficient_set);
  Scalar(size_t variable_set);
  Scalar(Rational coefficient_set, size_t variable_set);

  Rational get_coefficient() const;

  size_t Order() const;

  size_t VariableNumber() const;

  bool ContainsVariable(size_t const variable) const;
  size_t RemoveVariable(size_t const variable);

  void AddOther(Scalar const & other);
  bool CompareVariables(Scalar const & other) const;
  bool IsZero() const;

  void MultiplyCoefficient(Rational const & coeff);
  void DivideCoefficient(Rational const & coeff);

  Scalar MultiplyOther(Scalar const & other) const;

  std::string ToString(std::string base_name = "e_", bool plus_sign = false) const;

  void DivideByTwo();
  void Negate();

  bool operator== (Scalar const & other) const;
  bool operator< (Scalar const & other) const;

  ~Scalar() = default;
};

