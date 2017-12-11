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

class Expression {
 private:
  std::unique_ptr<Sum> summands;

 public:
  Expression() : summands(std::make_unique<std::vector<Summand>>()) { };

  Expression(Expression const & other) : summands(std::make_unique<Sum>()) {
    std::for_each(std::make_move_iterator(other.summands->begin()), std::make_move_iterator(other.summands->end()), [this](auto it) {
      summands->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
      auto monexp_new = std::make_unique<MonomialExpression>(*(it.first));
      auto scalar_new = std::make_unique<ScalarSum>(*(it.second));
      std::swap((*(--summands->end())).first, monexp_new);
      std::swap((*(--summands->end())).second, scalar_new);
    });
  };

  void AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar);
  void AddSummand (MonomialExpression const & monomial_expression);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational);
  void AddSummand (MonomialExpression const & monomial_expression, size_t variable);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, size_t variable);

  void ApplyMonomialSymmetries();
  void CanonicalisePrefactors();
  void CollectPrefactors();
  void EliminateZeros();
  void RedefineScalars();
  void SortMonomials();
  void SortSummands();
  void SortSummandsByPrefactors();

  void ExchangeSymmetrise(Indices const & indices_1, Indices const & indices_2, bool anti = false);

  bool ContainsMonomial (MonomialExpression const & monexpr) const;

  bool IsZero() const { return summands->empty(); };

  ScalarSum EvaluateIndices(Indices const & indices, std::vector<size_t> const & numbers) const;

  std::string const GetLatexString(std::string base_name = "e_", bool upper = true) const;

  ~Expression() = default;
};
