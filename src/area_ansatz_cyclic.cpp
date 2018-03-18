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
          
          Expression simplified_diff;

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          simplified_diff.MultiplyCoefficient(Rational(-1, 6));

          Expression simplified_new_int(expr);
          simplified_new_int.AddOther(simplified_diff);

          Expression simplified_diff_2;

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f', 'p', 'q'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          simplified_diff_2.MultiplyCoefficient(Rational(-1, 6));

          Expression expr_new(simplified_new_int);
          expr_new.AddOther(simplified_diff_2);

          expr_new.ApplyMonomialSymmetries();
          expr_new.SortMonomials();
          expr_new.SortSummands();
          expr_new.CollectPrefactors();
          expr_new.CanonicalisePrefactors();
          expr_new.EliminateZeros();

          expr = expr_new;
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
          
          Expression simplified_diff;

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (expr);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff.AddOther(expr_tmp);
          }

          simplified_diff.MultiplyCoefficient(Rational(-1, 6));

          Expression simplified_new_int(expr);
          simplified_new_int.AddOther(simplified_diff);

          Expression simplified_diff_2;

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g'}));
            expr_tmp.MultiplyCoefficient(Rational(1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          {
            Expression expr_tmp (simplified_new_int);
            expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                           Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f'}));
            expr_tmp.MultiplyCoefficient(Rational(-1, 1));
            simplified_diff_2.AddOther(expr_tmp);
          }

          simplified_diff_2.MultiplyCoefficient(Rational(-1, 6));

          Expression expr_new(simplified_new_int);
          expr_new.AddOther(simplified_diff_2);

          expr_new.ApplyMonomialSymmetries();
          expr_new.SortMonomials();
          expr_new.SortSummands();
          expr_new.CollectPrefactors();
          expr_new.CanonicalisePrefactors();
          expr_new.EliminateZeros();

          expr = expr_new;
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
