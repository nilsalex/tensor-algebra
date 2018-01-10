#include <sstream>

#include "Scalar.h"

Scalar::Scalar() : variables(), coefficient(1,1) { }
Scalar::Scalar(Scalar const & other) : variables(other.variables), coefficient(other.coefficient) { }

Scalar::Scalar(Rational const & coefficient_set) : variables(), coefficient(coefficient_set) { }
Scalar::Scalar(size_t variable_set) : variables(), coefficient(1,1) { variables.insert(variable_set); }
Scalar::Scalar(Rational coefficient_set, size_t variable_set) : variables(), coefficient(coefficient_set) { variables.insert(variable_set); }

Rational Scalar::get_coefficient() const { return coefficient; }

size_t Scalar::Order() const { return (IsZero() ? 0 : variables.size()); }

size_t Scalar::VariableNumber() const { assert(variables.size() == 1); return *(variables.begin()); }

bool Scalar::ContainsVariable(size_t const variable) const { return (std::find(variables.begin(), variables.end(), variable) != variables.end()); }

size_t Scalar::RemoveVariable(size_t const variable) {
  size_t old_size = variables.size();
  auto it = std::find(variables.begin(), variables.end(), variable);
  while (it != variables.end()) {
    variables.erase(it);
    it = std::find(variables.begin(), variables.end(), variable);
  }

  return old_size - variables.size();
}

void Scalar::AddOther(Scalar const & other) { assert(CompareVariables(other)); coefficient = coefficient.AddOther(other.coefficient); }
bool Scalar::CompareVariables(Scalar const & other) const { return variables == other.variables; }
bool Scalar::IsZero() const { return coefficient.IsZero(); }

void Scalar::MultiplyCoefficient(Rational const & coeff) { coefficient = coefficient.MultiplyOther(coeff); }
void Scalar::DivideCoefficient(Rational const & coeff) { coefficient = coefficient.DivideOther(coeff); }

void Scalar::DivideByTwo() { coefficient.DivideByTwo(); }
void Scalar::Negate() { coefficient.Negate(); }

Scalar Scalar::MultiplyOther(Scalar const & other) const {
  Scalar ret (*this);
  ret.MultiplyCoefficient(other.coefficient);
  std::for_each(other.variables.begin(), other.variables.end(), [&ret] (auto const & a) { ret.variables.insert(a); });
  return ret;
}

std::string Scalar::ToString(std::string base_name, bool plus_sign) const {
  std::stringstream ss;
  ss << coefficient.ToString(plus_sign);

  if (variables.empty() || IsZero()) {
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

