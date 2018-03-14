#include "Generator.h"
#include "Indices.h"
#include "Expression.h"
#include "TensorMonomial.h"

#include <fstream>
#include <iostream>

int main () {
  std::cout << "#############################################" << std::endl;
  std::cout << "Calculation of the most general ansatz for " << std::endl;
  std::cout << "the Lagrangian of area metric gravity." << std::endl;
  std::cout << std::endl;

    std::cout << " #### kinetic term ####" << std::endl;

  {
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
    
      Expression expr ( gtor.Generate() );
    
      std::cout << "Tensor ansatz:" << std::endl;
      std::cout << expr.GetLatexString() << std::endl;
    
      std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
      auto new_expr = std::unique_ptr<Expression>(new Expression(expr.NumericSimplify(indices, true)));
  }

    std::cout << " #### mass term ####" << std::endl;

  {
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
    
      Expression expr ( gtor.Generate() );
    
      std::cout << "Tensor ansatz:" << std::endl;
      std::cout << expr.GetLatexString() << std::endl;
    
      std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;

      auto new_expr = std::unique_ptr<Expression>(new Expression(expr.NumericSimplify(indices, true)));
  }
  

  return 0;
}
