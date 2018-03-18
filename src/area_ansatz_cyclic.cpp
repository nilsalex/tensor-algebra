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

      auto project_cyclic = [] (Expression & expr) -> void {
          
          auto simplified_diff = std::make_unique<Expression>();

          Indices free_indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'});
          std::vector<std::pair<Indices, Rational>> symmetries_1 {
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(-1, 1))
            };

          std::for_each(symmetries_1.cbegin(), symmetries_1.cend(),
            [&simplified_diff,free_indices,&expr] (auto const & sym) {
              Expression expr_tmp (expr);
              expr_tmp.SubstituteFreeIndices(free_indices, sym.first);
              expr_tmp.MultiplyCoefficient(sym.second);
              simplified_diff->AddOther(expr_tmp);
            });

          simplified_diff->MultiplyCoefficient(Rational(-1, 6));

          auto simplified_new = std::make_unique<Expression>(expr);
          simplified_new->AddOther(*simplified_diff);

          simplified_diff.reset(new Expression());

          std::vector<std::pair<Indices, Rational>> symmetries_2 {
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g', 'p', 'q'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h', 'p', 'q'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g', 'p', 'q'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f', 'p', 'q'}), Rational(-1, 1)),
            };

          std::for_each(symmetries_2.cbegin(), symmetries_2.cend(),
            [&simplified_diff,free_indices,&simplified_new] (auto const & sym) {
              Expression expr_tmp (*simplified_new);
              expr_tmp.SubstituteFreeIndices(free_indices, sym.first);
              expr_tmp.MultiplyCoefficient(sym.second);
              simplified_diff->AddOther(expr_tmp);
            });

          simplified_diff->MultiplyCoefficient(Rational(-1, 6));

          simplified_new->AddOther(*simplified_diff);
          simplified_diff.reset(nullptr);

          simplified_new->ApplyMonomialSymmetries();
          simplified_new->SortMonomials();
          simplified_new->SortSummands();
          simplified_new->CollectPrefactors();
          simplified_new->CanonicalisePrefactors();
          simplified_new->EliminateZeros();

          expr = *simplified_new;
        };
    
      Generator gtor (indices,  {std::make_pair(indices2, true),
                                 std::make_pair(indices3, true),
                                 std::make_pair(indices4, true),
                                 std::make_pair(indices5, true),
                                 std::make_pair(indices6, false),
                                 std::make_pair(indices7, false),
                                 std::make_pair(indices8, false),
                                 std::make_pair(indices9, false)},
                                {project_cyclic});
    
      Expression expr ( gtor.Generate() );
    
//      std::cout << "Tensor ansatz:" << std::endl;
//      std::cout << expr.GetLatexString() << std::endl;

      Expression simplified = expr.NumericSimplify(indices, true);

      std::cout << simplified.GetLatexString() << std::endl;
      simplified.SaveToFile("simplified_area_kinetic.prs");
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

      auto project_cyclic = [] (Expression & expr) -> void {
          
          auto simplified_diff = std::make_unique<Expression>();

          Indices free_indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'});
          std::vector<std::pair<Indices, Rational>> symmetries_1 {
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h'}), Rational(-1, 1))
            };

          std::for_each(symmetries_1.cbegin(), symmetries_1.cend(),
            [&simplified_diff,free_indices,&expr] (auto const & sym) {
              Expression expr_tmp (expr);
              expr_tmp.SubstituteFreeIndices(free_indices, sym.first);
              expr_tmp.MultiplyCoefficient(sym.second);
              simplified_diff->AddOther(expr_tmp);
            });

          simplified_diff->MultiplyCoefficient(Rational(-1, 6));

          auto simplified_new = std::make_unique<Expression>(expr);
          simplified_new->AddOther(*simplified_diff);

          simplified_diff.reset(new Expression());

          std::vector<std::pair<Indices, Rational>> symmetries_2 {
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h'}), Rational(-1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g'}), Rational(1, 1)),
              std::make_pair(Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f'}), Rational(-1, 1)),
            };

          std::for_each(symmetries_2.cbegin(), symmetries_2.cend(),
            [&simplified_diff,free_indices,&simplified_new] (auto const & sym) {
              Expression expr_tmp (*simplified_new);
              expr_tmp.SubstituteFreeIndices(free_indices, sym.first);
              expr_tmp.MultiplyCoefficient(sym.second);
              simplified_diff->AddOther(expr_tmp);
            });

          simplified_diff->MultiplyCoefficient(Rational(-1, 6));

          simplified_new->AddOther(*simplified_diff);
          simplified_diff.reset(nullptr);

          simplified_new->ApplyMonomialSymmetries();
          simplified_new->SortMonomials();
          simplified_new->SortSummands();
          simplified_new->CollectPrefactors();
          simplified_new->CanonicalisePrefactors();
          simplified_new->EliminateZeros();

          expr = *simplified_new;
        };
      Generator gtor (indices,  {std::make_pair(indices2, true),
                                 std::make_pair(indices3, true),
                                 std::make_pair(indices4, true),
                                 std::make_pair(indices5, true),
                                 std::make_pair(indices6, false),
                                 std::make_pair(indices7, false),
                                 std::make_pair(indices8, false)},
                                {project_cyclic});
    
      Expression expr ( gtor.Generate() );
    
      std::cout << "Tensor ansatz:" << std::endl;
      std::cout << expr.GetLatexString() << std::endl;
    
      Expression simplified = expr.NumericSimplify(indices, true);
      std::cout << simplified.GetLatexString() << std::endl;
      simplified.SaveToFile("simplified_area_mass.prs");
  }
  

  return 0;
}
