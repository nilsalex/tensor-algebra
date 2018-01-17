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

  Expression final_expr_kinetic;
  Expression final_expr_mass;

  {
    std::cout << " #### kinetic term ####" << std::endl;
    bool load_from_file = false;
    {
      std::ifstream f ("simplified_area_kinetic.prs");
      load_from_file = f.good();
    }

    Expression simplified;

    if (load_from_file) {
      simplified.LoadFromFile("simplified_area_kinetic.prs");
    } else {
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
  
      simplified.SaveToFile("simplified_area_kinetic.prs");
    }
  
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
  
      final_expr_kinetic = simplified.ApplyGaugeSymmetry(delta);
  }
  {
    std::cout << " #### mass term ####" << std::endl;
    bool load_from_file = false;
    {
      std::ifstream f ("simplified_area_mass.prs");
      load_from_file = f.good();
    }

    Expression simplified;

    if (load_from_file) {
      simplified.LoadFromFile("simplified_area_mass.prs");
    } else {
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
    
      std::cout << "Tensor ansatz:" << std::endl;
      std::cout << expr.GetLatexString() << std::endl;
    
      std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
      Expression simplified = expr.NumericSimplify(indices, true);
  
      simplified.SaveToFile("simplified_area_mass.prs");
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
  
      final_expr_mass = simplified.ApplyGaugeSymmetry(delta);
  }

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the kinetic " << std::endl;
  std::cout << "term of area metric gravity reads:" << std::endl;
  std::cout << final_expr_kinetic.GetLatexString() << std::endl;

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the mass " << std::endl;
  std::cout << "term of area metric gravity reads:" << std::endl;
  std::cout << final_expr_mass.GetLatexString() << std::endl;

  return 0;
}
