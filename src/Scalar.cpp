#include <sstream>

#include "Scalar.h"

std::string Scalar::ToString(std::string base_name, bool plus_sign) const {
  std::stringstream ss;
  ss << coefficient.ToString(plus_sign);

  if (variables.empty()) {
    return ss.str();
  }

  for (auto variable : variables) {
    ss << "*" << base_name << variable;
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
  return (variables == other.variables) && (coefficient == other.coefficient) || (IsZero() && other.IsZero());
};

