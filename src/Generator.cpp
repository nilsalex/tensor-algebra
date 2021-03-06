#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "Generator.h"
#include "MonomialExpression.h"
#include "MyPermutation.h"
#include "NextCombination.h"
#include "TensorMonomial.h"

Generator::Generator(Indices const & indices_set, std::initializer_list<std::pair<Indices, bool>> const & symmetries_set) : indices(indices_set), symmetries(symmetries_set), functions() {
  eta = Tensor(2, "eta");
  epsilon = Tensor(4, "epsilon");
  eta.SetSymmetric();
  epsilon.SetAntisymmetric();
}

Generator::Generator(Indices const & indices_set, std::initializer_list<std::pair<Indices, bool>> const & symmetries_set, std::initializer_list<std::function<void (Expression &)>> const & fun_set) : indices(indices_set), symmetries(symmetries_set), functions(fun_set) {
  eta = Tensor(2, "eta");
  epsilon = Tensor(4, "epsilon");
  eta.SetSymmetric();
  epsilon.SetAntisymmetric();
}

Expression Generator::Generate() {
  assert(indices.size() % 2 == 0);

  TensorMonomial T;
  for (size_t counter = 0; counter < indices.size() / 2; ++counter) {
    T.AddFactorRight(eta);
  }

  MyPermutation perm(indices.size());

  auto expression = std::make_unique<Expression>();
  size_t variable_counter = 0;

  do {
    Indices indices_perm = indices.Permutation(perm.GetPermutation());
    MonomialExpression monexpr(T, indices_perm);
    expression->AddSummand(monexpr, ++variable_counter);
    expression->SortMonomials();
  } while (perm.NextIndexPermutation());

  if (indices.size() > 3) {

    TensorMonomial S;
    S.AddFactorRight(epsilon);
    for (size_t counter = 0; counter < indices.size() / 2 - 2; ++counter) {
      S.AddFactorRight(eta);
    }

    std::vector<size_t> perm_epsilon(indices.size());
    std::vector<size_t> perm_indices(indices.size());
    std::generate(perm_epsilon.begin(), perm_epsilon.end(), [n=0]() mutable { return n++; });

    do {
      MyPermutation perm_eta(indices.size() - 4);
      perm_indices[0] = perm_epsilon[0];
      perm_indices[1] = perm_epsilon[1];
      perm_indices[2] = perm_epsilon[2];
      perm_indices[3] = perm_epsilon[3];
      do {
        for (size_t counter = 0; counter + 4 < indices.size(); ++counter) {
          perm_indices[counter + 4] = perm_epsilon[perm_eta.GetPermutation()[counter] + 4];
        }
        Indices indices_perm = indices.Permutation(perm_indices);
        MonomialExpression monexpr(S, indices_perm);

        expression->AddSummand(monexpr, ++variable_counter);
        expression->SortMonomials();
      } while (perm_eta.NextIndexPermutation());
    } while (next_combination(perm_epsilon.begin(), perm_epsilon.begin() + 4, perm_epsilon.end()));
  }


  std::for_each(symmetries.cbegin(), symmetries.cend(),
    [&expression,this] (auto symmetry) {
      expression->ExchangeSymmetrise(indices, symmetry.first, symmetry.second);
      expression->ApplyMonomialSymmetries();
      expression->SortMonomials();
      expression->SortSummands();
      expression->CollectPrefactors();
      expression->CanonicalisePrefactors();
      expression->EliminateZeros();
    });

  std::for_each(functions.cbegin(), functions.cend(),
    [&expression] (auto const & fun) {
      fun(*expression);
      expression->ApplyMonomialSymmetries();
      expression->SortMonomials();
      expression->SortSummands();
      expression->CollectPrefactors();
      expression->CanonicalisePrefactors();
      expression->EliminateZeros();
    });
      
  expression->SortSummandsByPrefactors();
  expression->RedefineScalars();

  return *expression;
} 
