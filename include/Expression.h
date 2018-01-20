#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

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

  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  Expression();
  Expression(Expression const & other); 
  Expression & operator=(Expression const & other);

  void AddSummand (MonomialExpression const & monomial_expression);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational);
  void AddSummand (MonomialExpression const & monomial_expression, size_t variable);
  void AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, size_t variable);
  void AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar);
  void AddSummand (MonomialExpression const & monomial_expression, ScalarSum const & scalar_sum);

  void ApplyMonomialSymmetries();
  void ApplyMonomialSymmetriesToContractions();
  void CanonicalisePrefactors();
  void CollectPrefactors();
  void EliminateVariable(size_t const variable);
  void SubstituteVariable(size_t const variable_old, ScalarSum const & scalar_sum_new);
  void SubstituteVariables(std::map<size_t, ScalarSum> substitution_map);
  void EliminateDelta();
  void EliminateEpsilon();
  void EliminateEtaEta();
  void EliminateEtaPartial();
  void EliminateZeros();
  void RedefineScalars();
  void RenameDummies();
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

  void SaveToFile(std::string const & filename) const;
  void LoadFromFile(std::string const & filename);

  std::string const GetLatexString(std::string base_name = "e_", bool upper = true) const;

  ~Expression() = default;
};
