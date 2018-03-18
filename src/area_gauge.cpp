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

  auto final_expr_kinetic = std::make_unique<Expression>();
  auto final_expr_mass = std::make_unique<Expression>();

  {
    std::cout << " #### kinetic term ####" << std::endl;
    bool load_from_file = false;
    {
      std::ifstream f ("simplified_area_kinetic.prs");
      load_from_file = f.good();
    }

    auto simplified = std::make_unique<Expression>();

    if (load_from_file) {
      simplified->LoadFromFile("simplified_area_kinetic.prs");
    } else {
      std::cout << "Input file \"simplified_area_kinetic.prs\" not found. Aborting." << std::endl;
      return -1;
    }
  
      std::cout << "Simplified ansatz without numerical linear dependencies (loaded from savefile):" << std::endl;
      std::cout << simplified->GetLatexString() << std::endl;

      Indices indices_delta   {'e', 'g', 'f', 'p', 'q', 'h'};
      Indices indices_delta2  {'e', 'g', 'h', 'p', 'q', 'f'};

      Indices indices_delta_epsilon {'e', 'f', 'g', 'm', 'p', 'q', 'h', 'm'};

      Tensor eta (2, "eta");
      eta.SetSymmetric();
      Tensor epsilon (4, "epsilonI");
      epsilon.SetAntisymmetric();
      Tensor partial (3, "partial");
      partial.SetSymmetric();
      Tensor xi      (1, "xi");
    
      TensorMonomial delta_tm;
      delta_tm.AddFactorRight(eta);
      delta_tm.AddFactorRight(partial);
      delta_tm.AddFactorRight(xi);
    
      MonomialExpression delta_me(delta_tm, indices_delta);

      TensorMonomial delta_tm_epsilon;
      delta_tm_epsilon.AddFactorRight(epsilon);
      delta_tm_epsilon.AddFactorRight(partial);
      delta_tm_epsilon.AddFactorRight(xi);

      MonomialExpression delta_me_epsilon(delta_tm_epsilon, indices_delta_epsilon);
    
      Expression delta;
      delta.AddSummand(delta_me, Rational(8, 1));
      delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

      auto new_expr = std::unique_ptr<Expression>(new Expression(simplified->ApplyGaugeSymmetry(delta)));
      std::swap(new_expr, final_expr_kinetic);
  }
  {
    std::cout << " #### mass term ####" << std::endl;
    bool load_from_file = false;
    {
      std::ifstream f ("simplified_area_mass.prs");
      load_from_file = f.good();
    }

    auto simplified = std::make_unique<Expression>();

    if (load_from_file) {
      simplified->LoadFromFile("simplified_area_mass.prs");
    } else {
      std::cout << "Input file \"simplified_area_mass.prs\" not found. Aborting." << std::endl;
      return -1;
    }
  
      std::cout << "Simplified ansatz without numerical linear dependencies (loaded from savefile):" << std::endl;
      std::cout << simplified->GetLatexString() << std::endl;

      Indices indices_delta   {'e', 'g', 'f', 'h'};
      Indices indices_delta2  {'e', 'g', 'h', 'f'};

      Indices indices_delta_epsilon {'e', 'f', 'g', 'm', 'h', 'm'};

      Tensor eta (2, "eta");
      eta.SetSymmetric();
      Tensor epsilon (4, "epsilonI");
      epsilon.SetAntisymmetric();
      Tensor partial (1, "partial");
      Tensor xi      (1, "xi");
    
      TensorMonomial delta_tm;
      delta_tm.AddFactorRight(eta);
      delta_tm.AddFactorRight(partial);
      delta_tm.AddFactorRight(xi);

      TensorMonomial delta_tm_epsilon;
      delta_tm_epsilon.AddFactorRight(epsilon);
      delta_tm_epsilon.AddFactorRight(partial);
      delta_tm_epsilon.AddFactorRight(xi);
    
      MonomialExpression delta_me(delta_tm, indices_delta);
      MonomialExpression delta_me_epsilon(delta_tm_epsilon, indices_delta_epsilon);
    
      Expression delta;
      delta.AddSummand(delta_me, Rational(8, 1));
    
      delta.ExchangeSymmetrise(indices_delta, indices_delta2, false);

      auto new_expr = std::unique_ptr<Expression>(new Expression(simplified->ApplyGaugeSymmetry(delta)));
      std::swap(new_expr, final_expr_mass);
  }

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the kinetic " << std::endl;
  std::cout << "term of area metric gravity reads:" << std::endl;
  std::cout << final_expr_kinetic->GetLatexString() << std::endl;

  std::cout << "#########################################" << std::endl;
  std::cout << "The most general ansatz for the mass " << std::endl;
  std::cout << "term of area metric gravity reads:" << std::endl;
  std::cout << final_expr_mass->GetLatexString("f_", true) << std::endl;

  final_expr_kinetic->SaveToFile("final_area_kinetic.prs");
  final_expr_mass->SaveToFile("final_area_mass.prs");

  {
      Expression final_diff;

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_kinetic);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      Expression final_new_int(final_diff);

      Expression final_diff_2;

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f', 'p', 'q'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      Expression final_new(final_new_int);
      final_new.AddOther(final_diff_2);

      final_new.ApplyMonomialSymmetries();
      final_new.SortMonomials();
      final_new.SortSummands();
      final_new.CollectPrefactors();
      final_new.CanonicalisePrefactors();
      final_new.EliminateZeros();

      std::cout << "########################################################################" << std::endl;
      std::cout << "anti cyclic part of final kinetic ansatz:" << std::endl;

      std::cout << final_new.GetLatexString() << std::endl;
  }

  {
      Expression final_diff;

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'c', 'b', 'd', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'c', 'd', 'b', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'd', 'b', 'c', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (*final_expr_mass);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'd', 'c', 'b', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff.AddOther(expr_tmp);
      }

      Expression final_new_int(final_diff);

      Expression final_diff_2;

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'f', 'h'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'g', 'h', 'f'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'f', 'g'}));
        expr_tmp.MultiplyCoefficient(Rational(1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      {
        Expression expr_tmp (final_new_int);
        expr_tmp.SubstituteFreeIndices(Indices ({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}),
                                       Indices ({'a', 'b', 'c', 'd', 'e', 'h', 'g', 'f'}));
        expr_tmp.MultiplyCoefficient(Rational(-1, 1));
        final_diff_2.AddOther(expr_tmp);
      }

      Expression final_new(final_new_int);
      final_new.AddOther(final_diff_2);

      final_new.ApplyMonomialSymmetries();
      final_new.SortMonomials();
      final_new.SortSummands();
      final_new.CollectPrefactors();
      final_new.CanonicalisePrefactors();
      final_new.EliminateZeros();

      std::cout << "########################################################################" << std::endl;
      std::cout << "anti cyclic part of final mass ansatz:" << std::endl;

      std::cout << final_new.GetLatexString() << std::endl;
  }

  return 0;
}
