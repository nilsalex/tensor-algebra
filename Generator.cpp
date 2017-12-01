#include <cassert>
#include <string>

#include "Generator.h"
#include "MonomialExpression.h"
#include "TensorMonomial.h"

Generator::Generator(Indices const & indices_set, std::initializer_list<std::pair<Indices, bool>> const & symmetries_set) : indices(indices_set), symmetries(symmetries_set), expression(Expression()) {
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

  expression.AddSummand(MonomialExpression(T, indices));

  return expression;
} 
