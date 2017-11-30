#include <sstream>

#include "Scalar.h"

std::string Scalar::ToString(bool plus_sign) const {
  std::stringstream ss;
  ss << coefficient.ToString(plus_sign);

  if (variables.empty()) {
    return ss.str();
  }

  for (auto variable : variables) {
    ss << "*" << variable;
  }

  return ss.str();
}

bool Scalar::operator< (Scalar const & other) const {
  if (variables < other.variables) {
    return true;
  } else if (variables > other.variables) {
    return false;
  } else {
    return coefficient < other.coefficient;
  }
};

bool Scalar::operator== (Scalar const & other) const {
  return (variables == other.variables) && (coefficient == other.coefficient);
};

