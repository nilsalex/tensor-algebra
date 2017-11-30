#pragma once

#include "MonomialExpression.h"
#include "Rational.h"
#include "Scalar.h"
#include "ScalarSum.h"

typedef std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>> Summand;
typedef std::vector<std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>> Sum;

class Expression {
 private:
  std::unique_ptr<Sum> summands;

 public:
  Expression() : summands(std::make_unique<std::vector<Summand>>()) { };

  void AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar);
  void AddSummand (MonomialExpression const & monomial_expression);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational);
  void AddSummand (MonomialExpression const & monomial_expression, std::string variable);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, std::string variable);

  void ApplyMonomialSymmetries();
  void CanonicalisePrefactors();
  void CollectPrefactors();
  void EliminateZeros();
  void SortMonomials();
  void SortSummands();

  void ExchangeSymmetrise(Indices const & indices_1, Indices const & indices_2, bool anti = false);

  std::string const GetLatexString(bool upper = true) const;

  ~Expression() = default;
};
