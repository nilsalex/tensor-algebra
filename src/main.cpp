#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <fstream>
#include <iostream>

int main () {
  bool load_from_file = false;
  {
    std::ifstream f ("simplified_area_massive.prs");
    load_from_file = f.good();
  }

  Expression simplified;

    Indices indices  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  if (!load_from_file) {
  
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
  
    Expression expr = gtor.Generate();
  
    std::cout << "Tensor ansatz:" << std::endl;
    std::cout << expr.GetLatexString() << std::endl;
  
    std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
    simplified = expr.NumericSimplify(indices, true);
  
    simplified.SaveToFile("simplified_area_massive.prs");
  } else {
    simplified.LoadFromFile("simplified_area_massive.prs");
  }

  std::cout << "Simplified ansatz:" << std::endl;
  std::cout << simplified.GetLatexString() << std::endl;

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

  Expression finalexpr = simplified.ApplyGaugeSymmetry(delta);

  std::cout << finalexpr.GetLatexString() << std::endl;

  finalexpr.NumericSimplify(indices, true);

  return 0;
}
