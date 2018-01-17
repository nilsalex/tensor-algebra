#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "Scalar.h"

class ScalarSum {
 private:
  std::unique_ptr<std::vector<std::unique_ptr<Scalar>>> scalars;

  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);
 
 public:
  ScalarSum();
  ScalarSum(ScalarSum const & other);
  ScalarSum(Scalar const & scalar);

  bool IsZero() const;

  std::string ToString(std::string base_name = "e_", bool plus_sign = false) const;
  void Negate();
  void DivideByTwo();

  void DivideCoefficient(Rational const & coeff);
  void MultiplyCoefficient(Rational const & coeff);

  Rational Ratio(ScalarSum const & other) const;

  void AddScalar(Scalar const & other);
  void EliminateVariable(size_t const variable);
  void SubstituteVariable(size_t const variable, ScalarSum const & scalar_sum_new);
  void MergeWithOther(ScalarSum const & other);
  void Sort();
  void Collect();

  ScalarSum MultiplyOther (ScalarSum const & other) const;

  std::set<size_t> CoefficientSet() const;
  std::vector<Rational> CoefficientVector(std::map<size_t, size_t> const & coefficient_map) const;

  ScalarSum & operator=(ScalarSum const & other);

  bool operator== (ScalarSum const & other) const;
  bool operator!= (ScalarSum const & other) const;
  bool operator< (ScalarSum const & other) const;

  ~ScalarSum() = default;
};
