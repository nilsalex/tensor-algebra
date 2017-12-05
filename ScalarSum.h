#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Scalar.h"

class ScalarSum {
 private:
  std::unique_ptr<std::vector<std::unique_ptr<Scalar>>> scalars;
 
 public:
  ScalarSum() : scalars(std::make_unique<std::vector<std::unique_ptr<Scalar>>>()) { scalars->push_back(std::make_unique<Scalar>()); } ;
  ScalarSum(ScalarSum const & other);
  ScalarSum(Scalar const & scalar) : scalars(std::make_unique<std::vector<std::unique_ptr<Scalar>>>()) { scalars->push_back(std::make_unique<Scalar>(scalar)); };

  bool IsZero() const;

  std::string ToString(bool plus_sign = false) const;
  void Negate() { for (auto & scalar : *scalars) { scalar->Negate(); } }; 
  void DivideByTwo() { for (auto & scalar : *scalars) { scalar->DivideByTwo(); } }; 

  void DivideCoefficient(Rational const & coeff);

  Rational Ratio(ScalarSum const & other) const;

  void MergeWithOther(ScalarSum const & other) { scalars->insert(scalars->end(), std::make_move_iterator(other.scalars->begin()), std::make_move_iterator(other.scalars->end())); };

  void Sort();
  void Collect();

  bool operator== (ScalarSum const & other) const;
  bool operator< (ScalarSum const & other) const;

  ~ScalarSum() = default;
};
