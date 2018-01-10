#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
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

  Expression expr = gtor.Generate();

  expr.EliminateVariable(1);
  expr.EliminateVariable(2);
  expr.EliminateVariable(3);
  expr.EliminateVariable(4);
  expr.EliminateVariable(5);
  expr.EliminateVariable(7);

  expr.EliminateZeros();

  expr.SubstituteVariable(6, ScalarSum(Scalar(Rational(1,1))));
  expr.SubstituteVariable(8, ScalarSum(Scalar(Rational(-2,1))));
  expr.SubstituteVariable(9, ScalarSum(Scalar(Rational(4,1))));

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  size_t counter = 0;
  size_t max_count = 4*4*4*4*4*4*4*4;

  for (size_t c1 = 0; c1 < 4; ++c1) {
    for (size_t c2 = 0; c2 < 4; ++c2) {
      for (size_t c3 = 0; c3 < 4; ++c3) {
        for (size_t c4 = 0; c4 < 4; ++c4) {
          for (size_t c5 = 0; c5 < 4; ++c5) {
            for (size_t c6 = 0; c6 < 4; ++c6) {
              for (size_t c7 = 0; c7 < 4; ++c7) {
                for (size_t c8 = 0; c8 < 4; ++c8) {
                  
  if ( expr.EvaluateIndices(indices, std::vector<size_t> ( { c1, c2, c3, c4, c5, c6, c7, c8} )).ToString() != "0" ) {
    std::cout << "M^{ " << c1 << " " << c2 << " " << c3 << " " << c4 << " " << c5 << " " << c6 << " " << c7 << " " << c8 << " } is not zero!" << std::endl;
  }

  ++counter;

  if (counter % 10000 == 0) {
    std::cout << counter << " / " << max_count << std::endl;
  }

                }
              }
            }
          }
        }
      }
    }
  }

  Tensor epsilon (4, "epsilon");
  Indices i {'a', 'f', 'b', 'x' };

  MonomialExpression epsilon_me (epsilon, i);

  Expression epsilon_ex;
  epsilon_ex.AddSummand(epsilon_me);

  std::cout << epsilon_ex.GetLatexString() << std::endl;

  epsilon_ex.EliminateEpsilon();

  std::cout << epsilon_ex.GetLatexString() << std::endl;

  return 0;
}
