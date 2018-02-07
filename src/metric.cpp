#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
  std::cout << "#############################################" << std::endl;
  std::cout << "Calculation of the most general ansatz for " << std::endl;
  std::cout << "the Lagrangian of metric gravity." << std::endl;
  std::cout << std::endl;
  std::cout << " #### kinetic term ####" << std::endl;

  auto final_expr_kinetic = std::make_unique<Expression>();
  auto final_expr_mass = std::make_unique<Expression>();

  {
  Indices indices  {'a', 'b', 'c', 'd', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'p', 'q'};
  Indices indices4 {'c', 'd', 'a', 'b', 'p', 'q'};
  Indices indices5 {'a', 'b', 'c', 'd', 'q', 'p'};

  Generator gtor (indices,  {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false),
                             std::make_pair(indices5, false),
                             });

  Expression expr = gtor.Generate();

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
  Expression simplified = expr.NumericSimplify(indices, true);

  std::cout << "Simplified ansatz:" << std::endl;
  std::cout << simplified.GetLatexString() << std::endl;

  Indices indices_delta   {'c', 'p', 'q', 'd'};

  Indices indices_delta2  {'d', 'p', 'q', 'c'};

  Tensor partial (3, "partial");
  partial.SetSymmetric();
  Tensor xi      (1, "xi");

  TensorMonomial delta_tm;
  delta_tm.AddFactorRight(partial);
  delta_tm.AddFactorRight(xi);

  MonomialExpression delta_me(delta_tm, indices_delta);

  Expression delta;
  delta.AddSummand(delta_me);

  delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

  final_expr_kinetic = std::unique_ptr<Expression>(new Expression(simplified.ApplyGaugeSymmetry(delta)));
  }
  {
  std::cout << " ### mass term ###" << std::endl;
  Indices indices  {'a', 'b', 'c', 'd'};

  Indices indices2 {'b', 'a', 'c', 'd'};
  Indices indices3 {'a', 'b', 'd', 'c'};
  Indices indices4 {'c', 'd', 'a', 'b'};

  Generator gtor (indices,  {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false),
                             });

  Expression expr = gtor.Generate();

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
  Expression simplified = expr.NumericSimplify(indices, true);

  std::cout << "Simplified ansatz:" << std::endl;
  std::cout << simplified.GetLatexString() << std::endl;

  Indices indices_delta   {'c', 'd'};

  Indices indices_delta2  {'d', 'c'};

  Tensor partial (1, "partial");
  Tensor xi      (1, "xi");

  TensorMonomial delta_tm;
  delta_tm.AddFactorRight(partial);
  delta_tm.AddFactorRight(xi);

  MonomialExpression delta_me(delta_tm, indices_delta);

  Expression delta;
  delta.AddSummand(delta_me);

  delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

  final_expr_mass = std::unique_ptr<Expression>(new Expression(simplified.ApplyGaugeSymmetry(delta)));
  }

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the kinetic " << std::endl;
  std::cout << "term of metric gravity reads:" << std::endl;
  std::cout << final_expr_kinetic->GetLatexString() << std::endl;

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the mass " << std::endl;
  std::cout << "term of metric gravity reads:" << std::endl;
  std::cout << final_expr_mass->GetLatexString() << std::endl;

  return 0;
}
