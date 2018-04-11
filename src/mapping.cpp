#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <fstream>
#include <iostream>

int main () {
  std::cout << "#############################################" << std::endl;
  std::cout << "Calculation of the most general ansatz for " << std::endl;
  std::cout << "a linear mapping between two area metrics" << std::endl;
  std::cout << "#############################################" << std::endl;
  std::cout << std::endl;

  Indices indices  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  Indices indices2 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h'};
  Indices indices3 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'};
  Indices indices4 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h'};
  Indices indices5 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'};

  Indices indices6 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h'};
  Indices indices7 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f'};


  Generator gtor (indices,  {std::make_pair(indices2, true),
                             std::make_pair(indices3, true),
                             std::make_pair(indices4, true),
                             std::make_pair(indices5, true),
                             std::make_pair(indices6, false),
                             std::make_pair(indices7, false)});

  Expression expr ( gtor.Generate() );

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  Expression simplified = expr.NumericSimplify(indices, true);
  std::cout << simplified.GetLatexString() << std::endl;

  return 0;
}
