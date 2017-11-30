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

  bool swapped;
  do {
    swapped = false;
    for (auto it = scalars->begin(); std::distance(it, scalars->end()) > 1; ++it) {
      if (**it < **std::next(it)) {
        std::swap(*it, *std::next(it));
        swapped = true;
      }
    }
  } while (swapped);
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

