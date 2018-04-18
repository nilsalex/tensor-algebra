#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
  std::cout << "#############################################" << std::endl;
  std::cout << "Calculation of the most general ansatz for " << std::endl;
  std::cout << "the Lagrangian of area metric gravity. 2nd order." << std::endl;
  std::cout << std::endl;

    std::cout << " #### kinetic term ####" << std::endl;

  {
      Indices indices  {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

      Generator gtor (indices, {});
    
      Expression expr ( gtor.Generate() );
    
      std::cout << "Tensor ansatz:" << std::endl;
      std::cout << expr.GetLatexString() << std::endl;
  }

  return 0;
}
