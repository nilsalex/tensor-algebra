#include <algorithm>
#include <sstream>

#include "Scalar.h"
#include "ScalarSum.h"

ScalarSum::ScalarSum(ScalarSum const & other) : scalars(std::make_unique<std::vector<std::unique_ptr<Scalar>>>()) { 
  for (auto & scalar : *(other.scalars)) {
    auto scalar_new = std::make_unique<Scalar>(*scalar);
    scalars->push_back(std::make_unique<Scalar>());
    std::swap(*(--scalars->end()), scalar_new);
  }
};

std::string ScalarSum::ToString(bool plus_sign) const {
  if (scalars->size() == 1) {
    return scalars->at(0)->ToString(plus_sign);
  }

  std::stringstream ss;
  if (plus_sign) {
    ss << "+";
  }
  ss << " (";
  bool first = true;
  for (auto & scalar : *scalars) {
    if (!first) {
      ss << " ";
    }
    ss << scalar->ToString(!first);
    first = false;
  }
  ss << ")";
  return ss.str();
}

void ScalarSum::Sort() {
  for (auto & scalar : *scalars) {
    scalar->Sort();
  }

  std::sort(scalars->begin(), scalars->end(), 
    [](auto & a, auto & b) {
      return *a < *b;
    });
}

void ScalarSum::Collect() {
  if (scalars->size() < 2) {
    return;
  }

  auto scalars_new = std::make_unique<std::vector<std::unique_ptr<Scalar>>>();

  auto last = scalars->begin();
  for (auto it = scalars->begin() + 1; it < scalars->end(); ++it) {
    if ((*last)->CompareVariables(**it)) {
      (*last)->AddOther(**it);
      if (std::distance(it, scalars->end()) == 1 && !(*last)->IsZero()) {
        scalars_new->push_back(std::make_unique<Scalar>());
        std::swap(*(--(scalars_new->end())), *last);
      }
    } else {
        if (!(*last)->IsZero()) {
          scalars_new->push_back(std::make_unique<Scalar>());
          std::swap(*(--(scalars_new->end())), *last);
        }
      if (std::distance(it, scalars->end()) == 1) {
        scalars_new->push_back(std::make_unique<Scalar>());
        std::swap(*(--(scalars_new->end())), *it);
      } else {
        last = it;
      }
    }
  }

  std::swap(scalars, scalars_new);
}

bool ScalarSum::IsZero() const {
  if (scalars->empty()) {
    return true;
  }

  bool zero = true;
  for (auto & scalar : *scalars) {
    if (!(scalar->IsZero())) {
      zero = false;
    }
  }

  return zero;
}

void ScalarSum::DivideCoefficient(Rational const & coeff) {
  std::for_each(scalars->begin(), scalars->end(), 
    [&coeff](auto & a) {
      a->DivideCoefficient(coeff);
    });
}

void ScalarSum::MultiplyCoefficient(Rational const & coeff) {
  if (coeff.IsZero()) {
    auto scalars_new = std::make_unique<std::vector<std::unique_ptr<Scalar>>>();
    std::swap(scalars, scalars_new);
  } else {
  std::for_each(scalars->begin(), scalars->end(), 
    [&coeff](auto & a) {
      a->MultiplyCoefficient(coeff);
    });
  }
}

Rational ScalarSum::Ratio(ScalarSum const & other) const {
  return Rational (scalars->front()->get_coefficient().DivideOther(other.scalars->front()->get_coefficient()));
}

std::set<size_t> ScalarSum::CoefficientSet() const {
  std::set<size_t> ret;
  std::for_each(scalars->begin(), scalars->end(),
    [&ret](auto & a) {
      ret.insert(a->VariableNumber());
    });

  return ret;
}

std::vector<Rational> ScalarSum::CoefficientVector(std::map<size_t, size_t> const & coefficient_map) const {
  std::vector<Rational> ret(coefficient_map.size());
  std::for_each(scalars->begin(), scalars->end(),
    [&ret,&coefficient_map](auto & a) {
      ret[coefficient_map.at(a->VariableNumber())] = a->get_coefficient();
    });

  return ret;
}

bool ScalarSum::operator==(ScalarSum const & other) const {
  Rational ratio (scalars->front()->get_coefficient().DivideOther(other.scalars->front()->get_coefficient()));
  return std::equal(scalars->cbegin(), scalars->cend(), other.scalars->cbegin(), other.scalars->cend(),
    [&ratio](auto & a, auto & b) {
      Scalar b_tmp = *b;
      b_tmp.MultiplyCoefficient(ratio);
      return *a == b_tmp;
    });
}

bool ScalarSum::operator<(ScalarSum const & other) const {
  return std::lexicographical_compare(this->scalars->cbegin(), this->scalars->cend(), other.scalars->cbegin(), other.scalars->cend(),
    [] (auto & a, auto & b) {
      return *a < *b;
    });
}
