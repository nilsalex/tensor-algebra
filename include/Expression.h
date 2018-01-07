#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <vector>

#include "MonomialExpression.h"
#include "Rational.h"
#include "Scalar.h"
#include "ScalarSum.h"

typedef std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>> Summand;
typedef std::vector<std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>> Sum;

bool operator< (Summand const & lop, Summand const & rop);

class Expression {
 private:
  std::unique_ptr<Sum> summands;

 public:
  Expression();
  Expression(Expression const & other); 

  void AddSummand (MonomialExpression const & monomial_expression);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational);
  void AddSummand (MonomialExpression const & monomial_expression, size_t variable);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, size_t variable);
  void AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar);

  void ApplyMonomialSymmetries();
  void CanonicalisePrefactors();
  void CollectPrefactors();
  void EliminateVariable(size_t const variable);
  void EliminateZeros();
  void RedefineScalars();
  void SortMonomials();
  void SortSummands();
  void SortSummandsByPrefactors();

  Expression ApplyGaugeSymmetry (Expression const & delta) const;

  Expression NumericSimplify (Indices const & indices, bool print_matrix = false) const;

  Expression MultiplyOther (Expression const & other) const;

  void ExchangeSymmetrise(Indices const & indices_1, Indices const & indices_2, bool anti = false);

  bool ContainsMonomial (MonomialExpression const & monexpr) const;

  bool IsZero() const;

  ScalarSum EvaluateIndices(Indices const & indices, std::vector<size_t> const & numbers) const;

  std::string const GetLatexString(std::string base_name = "e_", bool upper = true) const;

  ~Expression() = default;
};
