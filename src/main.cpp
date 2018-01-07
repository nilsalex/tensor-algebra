#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
/*
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
*/  

  Indices indices  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  Indices indices2 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h'};
  Indices indices3 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'};
  Indices indices4 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h'};
  Indices indices5 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'};

  Indices indices6 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h'};
  Indices indices7 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f'};

  Indices indices8 {'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd'};

  Generator gtor (indices,  {std::make_pair(indices2, true),
                               std::make_pair(indices3, true),
                               std::make_pair(indices4, true),
                               std::make_pair(indices5, true),
                               std::make_pair(indices6, false),
                               std::make_pair(indices7, false),
                               std::make_pair(indices8, false)});

  Indices indices_delta   {'e', 'g', 'f', 'h'};

  Indices indices_delta2  {'e', 'g', 'h', 'f'};

  Tensor eta (2, "eta");
  eta.SetSymmetric();
  
  Tensor partial (1, "partial");
  Tensor xi      (1, "xi");

  TensorMonomial delta_tm;
  delta_tm.AddFactorRight(eta);
  delta_tm.AddFactorRight(partial);
  delta_tm.AddFactorRight(xi);

  MonomialExpression delta_me(delta_tm, indices_delta);

  Expression delta;
  delta.AddSummand(delta_me);

  delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

  std::cout << delta.GetLatexString() << std::endl;

/*
  Indices indices1 {'a', 'b', 'c', 'd', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'p', 'q'};
  Indices indices4 {'c', 'd', 'a', 'b', 'p', 'q'};
  Indices indices5 {'a', 'b', 'c', 'd', 'q', 'p'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false),
                             std::make_pair(indices5, false)});
*/
/*
  Indices indices1 {'a', 'b', 'c', 'd', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'p', 'q'};
  Indices indices4 {'a', 'b', 'c', 'd', 'q', 'p'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false)});
*/                             
/*
  Indices indices1 {'a', 'b', 'c', 'd'};
  Indices indices2 {'b', 'a', 'c', 'd'};
  Indices indices3 {'a', 'b', 'd', 'c'};
  Indices indices4 {'c', 'd', 'a', 'b'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false)});
*/
/*
  Indices indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'};

  Generator gtor (indices, {});
*/
  Expression expr = gtor.Generate();

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
  Expression simplified = expr.NumericSimplify(indices, true);

  std::cout << "Simplified ansatz:" << std::endl;
  std::cout << simplified.GetLatexString() << std::endl;

  Expression gauge_term = simplified.ApplyGaugeSymmetry(delta);

  std::cout << "Gauge term:" << std::endl;
  std::cout << gauge_term.GetLatexString() << std::endl;

  return 0;
}
