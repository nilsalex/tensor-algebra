#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
  Indices indices  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices4 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h', 'p', 'q'};
  Indices indices5 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g', 'p', 'q'};

  Indices indices6 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices7 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f', 'p', 'q'};

  Indices indices8 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'q', 'p'};

  Indices indices9 {'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'p', 'q'};

  Generator gtor (indices,  {std::make_pair(indices2, true),
                             std::make_pair(indices3, true),
                             std::make_pair(indices4, true),
                             std::make_pair(indices5, true),
                             std::make_pair(indices6, false),
                             std::make_pair(indices7, false),
                             std::make_pair(indices8, false),
                             std::make_pair(indices9, false)});

  Expression expr = gtor.Generate();

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
  Expression simplified = expr.NumericSimplify(indices, true);

  std::cout << "Simplified ansatz:" << std::endl;
  std::cout << simplified.GetLatexString() << std::endl;

  Indices indices_delta   {'e', 'g', 'f', 'p', 'q', 'h'};

  Indices indices_delta2  {'e', 'g', 'h', 'p', 'q', 'f'};

  Tensor eta (2, "eta");
  eta.SetSymmetric();
  Tensor partial (3, "partial");
  partial.SetSymmetric();
  Tensor xi      (1, "xi");

  TensorMonomial delta_tm;
  delta_tm.AddFactorRight(eta);
  delta_tm.AddFactorRight(partial);
  delta_tm.AddFactorRight(xi);

  MonomialExpression delta_me(delta_tm, indices_delta);

  Expression delta;
  delta.AddSummand(delta_me);

  delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

  Expression gauge_term = expr.ApplyGaugeSymmetry(delta);
  
  return 0;
}
