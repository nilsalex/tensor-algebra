#include "Expression.h"
#include "TensorMonomial.h"

#include <fstream>
#include <iostream>

int main() {
  Expression ansatz_mass;
  Expression ansatz_kinetic;

  // Loading save ansätze
  ansatz_mass.LoadFromFile("final_area_mass.prs");
  ansatz_kinetic.LoadFromFile("final_area_kinetic.prs");

  // defining tensors for three plus one decomposition of the dynamic fields
  Tensor A(0, "A", 50);
  Tensor B(1, "B", 60);
  Tensor phi1(2, "phi1", 20);
  phi1.SetSymmetric();
  Tensor phi2(2, "phi2", 25);
  phi2.SetSymmetric();
  Tensor phi3(2, "phi3", 30);
  phi3.SetSymmetric();
  phi3.SetTracefree();
  Tensor gamma(2, "gamma");
  gamma.SetSymmetric();
  Tensor delta(2, "delta");
  delta.SetSymmetric();
  Tensor epsilon(3, "epsilon");
  epsilon.SetAntisymmetric();
  Tensor partial_t(0, "partial_t");
  Tensor partial(1, "partial");
  Tensor partial2(2, "partial");
  partial2.SetSymmetric();

  MonomialExpression me_gamma_A (TensorMonomial ({gamma, A}), Indices({'m', 'n'}));
  MonomialExpression me_phi1 (phi1, Indices({'m', 'n'}));

  MonomialExpression me_gamma_B (TensorMonomial ({gamma, B}), Indices({'m', 'n', 'p'}));
  MonomialExpression me_gamma_phi1_epsilon (TensorMonomial ({gamma, phi1, epsilon}), Indices({'u', 'v', 'u', 'v', 'm', 'n', 'p'}));
  MonomialExpression me_A_epsilon (TensorMonomial ({A, epsilon}), Indices({'m', 'n', 'p'}));
  MonomialExpression me_phi3_epsilon (TensorMonomial ({phi3, epsilon}), Indices({'m', 'u', 'n', 'p', 'u'}));

  MonomialExpression me_B_epsilon (TensorMonomial ({B, epsilon}), Indices({'m', 'n', 'p', 'q'}));
  MonomialExpression me_gamma_phi1_gamma_gamma (TensorMonomial ({gamma, phi1, gamma, gamma}), Indices ({'u', 'v', 'u', 'v', 'm', 'p', 'q', 'n'}));
  MonomialExpression me_epsilon_epsilon_phi2 (TensorMonomial ({epsilon, epsilon, phi2}), Indices ({'m', 'n', 'u', 'p', 'q', 'v', 'u', 'v'}));

  MonomialExpression me_partial_t_t (TensorMonomial ({partial_t, partial_t}), Indices ());
  MonomialExpression me_partial_t_r(TensorMonomial ({partial_t, partial}), Indices ({'r'}));
  MonomialExpression me_partial_r_s(partial2, Indices ({'r', 's'}));

  MonomialExpression me_epsilon_xbc (TensorMonomial ({epsilon}), Indices ({'x', 'b', 'c'}));

  Expression E_0m0n;
  E_0m0n.set_dimension(3);
  E_0m0n.AddSummand(me_gamma_A, Rational(2,1));
  E_0m0n.AddSummand(me_phi1, Rational(-1,1));

  Expression E_0mnp;
  E_0mnp.set_dimension(3);
  E_0mnp.AddSummand(me_gamma_B, Rational(-1, 1));
  me_gamma_B.ExchangeIndices(Indices({'m', 'n', 'p'}), Indices({'m', 'p', 'n'}));
  E_0mnp.AddSummand(me_gamma_B, Rational(1, 1));
  E_0mnp.AddSummand(me_gamma_phi1_epsilon, Rational(1, 2));
  E_0mnp.AddSummand(me_A_epsilon, Rational(-1, 1));
  E_0mnp.AddSummand(me_phi3_epsilon, Rational(1,1));

  Expression E_mnpq;
  E_mnpq.set_dimension(3);
  E_mnpq.AddSummand(me_B_epsilon, Rational(-1, 1));
  me_B_epsilon.ExchangeIndices(Indices({'m', 'n', 'p', 'q'}), Indices({'n', 'm', 'p', 'q'}));
  E_mnpq.AddSummand(me_B_epsilon, Rational(1, 1));
  me_B_epsilon.ExchangeIndices(Indices({'n', 'm', 'p', 'q'}), Indices({'p', 'q', 'm', 'n'}));
  E_mnpq.AddSummand(me_B_epsilon, Rational(-1, 1));
  me_B_epsilon.ExchangeIndices(Indices({'p', 'q', 'm', 'n'}), Indices({'q', 'p', 'm', 'n'}));
  E_mnpq.AddSummand(me_B_epsilon, Rational(1, 1));
  E_mnpq.AddSummand(me_gamma_phi1_gamma_gamma, Rational(1, 1));
  me_gamma_phi1_gamma_gamma.ExchangeIndices(Indices({'u', 'v', 'u', 'v', 'm', 'p', 'q', 'n'}), Indices({'u', 'v', 'u', 'v', 'm', 'q', 'p', 'n'}));
  E_mnpq.AddSummand(me_gamma_phi1_gamma_gamma, Rational(-1, 1));
  E_mnpq.AddSummand(me_epsilon_epsilon_phi2, Rational(1,1));

  Expression partial_t_t;
  partial_t_t.set_dimension(3);
  partial_t_t.AddSummand(me_partial_t_t, Rational(1,1));

  Expression partial_t_r;
  partial_t_r.set_dimension(3);
  partial_t_r.AddSummand(me_partial_t_r, Rational(1,1));

  Expression partial_r_s;
  partial_r_s.set_dimension(3);
  partial_r_s.AddSummand(me_partial_r_s, Rational(1,1));

  Expression epsilon_xbc;
  epsilon_xbc.set_dimension(3);
  epsilon_xbc.AddSummand(me_epsilon_xbc, Rational(1, 2));

  Expression E_0m0n00 ( E_0m0n );
  E_0m0n00.MultiplyOther(partial_t_t);
  Expression E_0m0n0r ( E_0m0n );
  E_0m0n0r.MultiplyOther(partial_t_r);
  Expression E_0m0nrs ( E_0m0n );
  E_0m0nrs.MultiplyOther(partial_r_s);

  Expression E_0mnp00 ( E_0mnp );
  E_0mnp00.MultiplyOther(partial_t_t);
  Expression E_0mnp0r ( E_0mnp );
  E_0mnp0r.MultiplyOther(partial_t_r);
  Expression E_0mnprs ( E_0mnp );
  E_0mnprs.MultiplyOther(partial_r_s);

  Expression E_mnpq00 ( E_mnpq );
  E_mnpq00.MultiplyOther(partial_t_t);
  Expression E_mnpq0r ( E_mnpq );
  E_mnpq0r.MultiplyOther(partial_t_r);
  Expression E_mnpqrs ( E_mnpq );
  E_mnpqrs.MultiplyOther(partial_r_s);

  // ################################################################################

  Expression ansatz_mass_0abc0m0n (ansatz_mass);

  ansatz_mass_0abc0m0n.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::g}));
  ansatz_mass_0abc0m0n.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n'}));

  Expression EQ_abc_1 ( ansatz_mass_0abc0m0n );
  EQ_abc_1.MultiplyOther(E_0m0n);

  EQ_abc_1.EliminateGamma();
  EQ_abc_1.RenameDummies();
  EQ_abc_1.ApplyMonomialSymmetries();
  EQ_abc_1.SortMonomials();
  EQ_abc_1.SortSummands();
  EQ_abc_1.CollectPrefactors();
  EQ_abc_1.CanonicalisePrefactors();
  EQ_abc_1.EliminateZeros();

  // ################################################################################

  Expression ansatz_mass_0abc0mnp (ansatz_mass);

  ansatz_mass_0abc0mnp.ThreePlusOne(std::vector<Index> ({Index::a, Index::e}));
  ansatz_mass_0abc0mnp.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'g', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p'}));

  Expression EQ_abc_2 ( ansatz_mass_0abc0mnp );
  EQ_abc_2.MultiplyOther(E_0mnp);

  EQ_abc_2.EliminateEpsilonEpsilonI();
  EQ_abc_2.EliminateEpsilon();
  EQ_abc_2.EliminateGamma();
  EQ_abc_2.EliminateTracefree();
  EQ_abc_2.ApplyMonomialSymmetries();
  EQ_abc_2.SortMonomials();
  EQ_abc_2.ApplyMonomialSymmetriesToContractions();
  EQ_abc_2.RenameDummies();
  EQ_abc_2.SortSummands();
  EQ_abc_2.CollectPrefactors();
  EQ_abc_2.CanonicalisePrefactors();
  EQ_abc_2.EliminateZeros();

  // ################################################################################

  Expression ansatz_mass_0abcmnpq (ansatz_mass);

  ansatz_mass_0abcmnpq.ThreePlusOne(std::vector<Index> ({Index::a}));
  ansatz_mass_0abcmnpq.SubstituteIndices(Indices ({'b', 'c', 'd', 'e', 'f', 'g', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'q'}));

  Expression EQ_abc_3 ( ansatz_mass_0abcmnpq );
  EQ_abc_3.MultiplyOther(E_mnpq);

  EQ_abc_3.EliminateEpsilonEpsilonI();
  EQ_abc_3.EliminateGamma();
  EQ_abc_3.EliminateEpsilon();
  EQ_abc_3.EliminateTracefree();
  EQ_abc_3.EliminateZeros();
  EQ_abc_3.ApplyMonomialSymmetries();
  EQ_abc_3.SortMonomials();
  EQ_abc_3.ApplyMonomialSymmetriesToContractions();
  EQ_abc_3.RenameDummies();
  EQ_abc_3.SortSummands();
  EQ_abc_3.CollectPrefactors();
  EQ_abc_3.CanonicalisePrefactors();

  EQ_abc_1.MultiplyCoefficient(Rational(4, 1));
  EQ_abc_2.MultiplyCoefficient(Rational(4, 1));

  Expression EQ_abc ( EQ_abc_1 );
  EQ_abc.AddOther(EQ_abc_2);
  EQ_abc.AddOther(EQ_abc_3);

  Expression EQ_ab ( EQ_abc );
  EQ_ab.MultiplyOther(epsilon_xbc);

  EQ_ab.EliminateEpsilonEpsilonI();
  EQ_ab.EliminateGamma();
  EQ_ab.EliminateEpsilon();
  EQ_ab.EliminateTracefree();
  EQ_ab.EliminateZeros();
  EQ_ab.ApplyMonomialSymmetries();
  EQ_ab.SortMonomials();
  EQ_ab.ApplyMonomialSymmetriesToContractions();
  EQ_ab.RenameDummies();
  EQ_ab.SortSummandsByLastFactors();
  EQ_ab.CollectPrefactors();
  EQ_ab.CanonicalisePrefactors();

  std::cout << EQ_ab.GetLatexString("f_") << std::endl;

  // ################################################################################

  Expression ansatz_kinetic_0abc0m0n00 (ansatz_kinetic);

  ansatz_kinetic_0abc0m0n00.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::g, Index::p, Index::q}));
  ansatz_kinetic_0abc0m0n00.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n'}));

  Expression EQ_abc_00_1 ( ansatz_kinetic_0abc0m0n00 );
  EQ_abc_00_1.MultiplyOther(E_0m0n00);

  EQ_abc_00_1.EliminateEpsilonEpsilonI();
  EQ_abc_00_1.EliminateGamma();
  EQ_abc_00_1.EliminateEpsilon();
  EQ_abc_00_1.EliminateTracefree();
  EQ_abc_00_1.EliminateZeros();
  EQ_abc_00_1.ApplyMonomialSymmetries();
  EQ_abc_00_1.SortMonomials();
  EQ_abc_00_1.ApplyMonomialSymmetriesToContractions();
  EQ_abc_00_1.RenameDummies();
  EQ_abc_00_1.SortSummands();
  EQ_abc_00_1.CollectPrefactors();
  EQ_abc_00_1.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abc0mnp00 (ansatz_kinetic);

  ansatz_kinetic_0abc0mnp00.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::p, Index::q}));
  ansatz_kinetic_0abc0mnp00.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'g', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p'}));

  Expression EQ_abc_00_2 ( ansatz_kinetic_0abc0mnp00 );
  EQ_abc_00_2.MultiplyOther(E_0mnp00);

  EQ_abc_00_2.EliminateEpsilonEpsilonI();
  EQ_abc_00_2.EliminateGamma();
  EQ_abc_00_2.EliminateEpsilon();
  EQ_abc_00_2.EliminateTracefree();
  EQ_abc_00_2.EliminateZeros();
  EQ_abc_00_2.ApplyMonomialSymmetries();
  EQ_abc_00_2.SortMonomials();
  EQ_abc_00_2.ApplyMonomialSymmetriesToContractions();
  EQ_abc_00_2.RenameDummies();
  EQ_abc_00_2.SortSummands();
  EQ_abc_00_2.CollectPrefactors();
  EQ_abc_00_2.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abcmnpq00 (ansatz_kinetic);

  ansatz_kinetic_0abcmnpq00.ThreePlusOne(std::vector<Index> ({Index::a, Index::p, Index::q}));
  ansatz_kinetic_0abcmnpq00.SubstituteIndices(Indices ({'b', 'c', 'd', 'e', 'f', 'g', 'h'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'q'}));

  Expression EQ_abc_00_3 ( ansatz_kinetic_0abcmnpq00 );
  EQ_abc_00_3.MultiplyOther(E_mnpq00);

  EQ_abc_00_3.EliminateEpsilonEpsilonI();
  EQ_abc_00_3.EliminateGamma();
  EQ_abc_00_3.EliminateEpsilon();
  EQ_abc_00_3.EliminateTracefree();
  EQ_abc_00_3.EliminateZeros();
  EQ_abc_00_3.ApplyMonomialSymmetries();
  EQ_abc_00_3.SortMonomials();
  EQ_abc_00_3.ApplyMonomialSymmetriesToContractions();
  EQ_abc_00_3.RenameDummies();
  EQ_abc_00_3.SortSummands();
  EQ_abc_00_3.CollectPrefactors();
  EQ_abc_00_3.CanonicalisePrefactors();

  EQ_abc_00_1.MultiplyCoefficient(Rational(4, 1));
  EQ_abc_00_2.MultiplyCoefficient(Rational(4, 1));

  Expression EQ_abc_00 ( EQ_abc_00_1 );
  EQ_abc_00.AddOther(EQ_abc_00_2);
  EQ_abc_00.AddOther(EQ_abc_00_3);

  Expression EQ_ab_00 ( EQ_abc_00 );
  EQ_ab_00.MultiplyOther(epsilon_xbc);

  EQ_ab_00.EliminateEpsilonEpsilonI();
  EQ_ab_00.EliminateGamma();
  EQ_ab_00.EliminateEpsilon();
  EQ_ab_00.EliminateTracefree();
  EQ_ab_00.EliminateZeros();
  EQ_ab_00.ApplyMonomialSymmetries();
  EQ_ab_00.SortMonomials();
  EQ_ab_00.ApplyMonomialSymmetriesToContractions();
  EQ_ab_00.RenameDummies();
  EQ_ab_00.SortSummandsByLastFactors();
  EQ_ab_00.CollectPrefactors();
  EQ_ab_00.CanonicalisePrefactors();

  std::cout << "##########################################" << std::endl;

  std::cout << EQ_ab_00.GetLatexString() << std::endl;


  // ################################################################################

  Expression ansatz_kinetic_0abc0m0n0r (ansatz_kinetic);

  ansatz_kinetic_0abc0m0n0r.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::g, Index::p}));
  ansatz_kinetic_0abc0m0n0r.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'h', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'r'}));

  ansatz_kinetic_0abc0m0n0r.ApplyMonomialSymmetries();
  ansatz_kinetic_0abc0m0n0r.SortMonomials();
  ansatz_kinetic_0abc0m0n0r.SortSummands();
  ansatz_kinetic_0abc0m0n0r.CollectPrefactors();
  ansatz_kinetic_0abc0m0n0r.CanonicalisePrefactors();
  ansatz_kinetic_0abc0m0n0r.EliminateZeros();
  ansatz_kinetic_0abc0m0n0r.SortSummandsByPrefactors();


  std::cout << "*********************************************" << std::endl;
  std::cout << ansatz_kinetic_0abc0m0n0r.GetLatexString() << std::endl;
  std::cout << "*********************************************" << std::endl;

  Expression EQ_abc_0r_1 ( ansatz_kinetic_0abc0m0n0r );
  EQ_abc_0r_1.MultiplyOther(E_0m0n0r);

  EQ_abc_0r_1.EliminateEpsilonEpsilonI();
  EQ_abc_0r_1.EliminateGamma();
  EQ_abc_0r_1.EliminateEpsilon();
  EQ_abc_0r_1.EliminateTracefree();
  EQ_abc_0r_1.EliminateZeros();
  EQ_abc_0r_1.ApplyMonomialSymmetries();
  EQ_abc_0r_1.SortMonomials();
  EQ_abc_0r_1.ApplyMonomialSymmetriesToContractions();
  EQ_abc_0r_1.RenameDummies();
  EQ_abc_0r_1.SortSummands();
  EQ_abc_0r_1.CollectPrefactors();
  EQ_abc_0r_1.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abc0mnp0r (ansatz_kinetic);

  ansatz_kinetic_0abc0mnp0r.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::p}));
  ansatz_kinetic_0abc0mnp0r.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'g', 'h', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'r'}));

  Expression EQ_abc_0r_2 ( ansatz_kinetic_0abc0mnp0r );
  EQ_abc_0r_2.MultiplyOther(E_0mnp0r);

  EQ_abc_0r_2.EliminateEpsilonEpsilonI();
  EQ_abc_0r_2.EliminateGamma();
  EQ_abc_0r_2.EliminateEpsilon();
  EQ_abc_0r_2.EliminateTracefree();
  EQ_abc_0r_2.EliminateZeros();
  EQ_abc_0r_2.ApplyMonomialSymmetries();
  EQ_abc_0r_2.SortMonomials();
  EQ_abc_0r_2.ApplyMonomialSymmetriesToContractions();
  EQ_abc_0r_2.RenameDummies();
  EQ_abc_0r_2.SortSummands();
  EQ_abc_0r_2.CollectPrefactors();
  EQ_abc_0r_2.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abcmnpq0r (ansatz_kinetic);

  ansatz_kinetic_0abcmnpq0r.ThreePlusOne(std::vector<Index> ({Index::a, Index::p}));
  ansatz_kinetic_0abcmnpq0r.SubstituteIndices(Indices ({'b', 'c', 'd', 'e', 'f', 'g', 'h', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'q', 'r'}));

  Expression EQ_abc_0r_3 ( ansatz_kinetic_0abcmnpq0r );
  EQ_abc_0r_3.MultiplyOther(E_mnpq0r);

  EQ_abc_0r_3.EliminateEpsilonEpsilonI();
  EQ_abc_0r_3.EliminateGamma();
  EQ_abc_0r_3.EliminateEpsilon();
  EQ_abc_0r_3.EliminateTracefree();
  EQ_abc_0r_3.EliminateZeros();
  EQ_abc_0r_3.ApplyMonomialSymmetries();
  EQ_abc_0r_3.SortMonomials();
  EQ_abc_0r_3.ApplyMonomialSymmetriesToContractions();
  EQ_abc_0r_3.RenameDummies();
  EQ_abc_0r_3.SortSummands();
  EQ_abc_0r_3.CollectPrefactors();
  EQ_abc_0r_3.CanonicalisePrefactors();

  EQ_abc_0r_1.MultiplyCoefficient(Rational(8, 1));
  EQ_abc_0r_2.MultiplyCoefficient(Rational(8, 1));
  EQ_abc_0r_3.MultiplyCoefficient(Rational(2, 1));

  Expression EQ_abc_0r ( EQ_abc_0r_1 );
  EQ_abc_0r.AddOther(EQ_abc_0r_2);
  EQ_abc_0r.AddOther(EQ_abc_0r_3);

  Expression EQ_ab_0r ( EQ_abc_0r );
  EQ_ab_0r.MultiplyOther(epsilon_xbc);

  EQ_ab_0r.EliminateEpsilonEpsilonI();
  EQ_ab_0r.EliminateGamma();
  EQ_ab_0r.EliminateEpsilon();
  EQ_ab_0r.EliminateTracefree();
  EQ_ab_0r.EliminateZeros();
  EQ_ab_0r.ApplyMonomialSymmetries();
  EQ_ab_0r.SortMonomials();
  EQ_ab_0r.ApplyMonomialSymmetriesToContractions();
  EQ_ab_0r.RenameDummies();
  EQ_ab_0r.SortSummandsByLastFactors();
  EQ_ab_0r.CollectPrefactors();
  EQ_ab_0r.CanonicalisePrefactors();

  std::cout << "##########################################" << std::endl;

  std::cout << EQ_ab_0r.GetLatexString() << std::endl;



  // ################################################################################

  Expression ansatz_kinetic_0abc0m0nrs (ansatz_kinetic);

  ansatz_kinetic_0abc0m0nrs.ThreePlusOne(std::vector<Index> ({Index::a, Index::e, Index::g}));
  ansatz_kinetic_0abc0m0nrs.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'h', 'p', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'r', 's'}));

  Expression EQ_abc_rs_1 ( ansatz_kinetic_0abc0m0nrs );
  EQ_abc_rs_1.MultiplyOther(E_0m0nrs);

  EQ_abc_rs_1.EliminateEpsilonEpsilonI();
  EQ_abc_rs_1.EliminateGamma();
  EQ_abc_rs_1.EliminateEpsilon();
  EQ_abc_rs_1.EliminateTracefree();
  EQ_abc_rs_1.EliminateZeros();
  EQ_abc_rs_1.ApplyMonomialSymmetries();
  EQ_abc_rs_1.SortMonomials();
  EQ_abc_rs_1.ApplyMonomialSymmetriesToContractions();
  EQ_abc_rs_1.RenameDummies();
  EQ_abc_rs_1.SortSummands();
  EQ_abc_rs_1.CollectPrefactors();
  EQ_abc_rs_1.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abc0mnprs (ansatz_kinetic);

  ansatz_kinetic_0abc0mnprs.ThreePlusOne(std::vector<Index> ({Index::a, Index::e}));
  ansatz_kinetic_0abc0mnprs.SubstituteIndices(Indices ({'b', 'c', 'd', 'f', 'g', 'h', 'p', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'r', 's'}));

  Expression EQ_abc_rs_2 ( ansatz_kinetic_0abc0mnprs );
  EQ_abc_rs_2.MultiplyOther(E_0mnprs);

  EQ_abc_rs_2.EliminateEpsilonEpsilonI();
  EQ_abc_rs_2.EliminateGamma();
  EQ_abc_rs_2.EliminateEpsilon();
  EQ_abc_rs_2.EliminateTracefree();
  EQ_abc_rs_2.EliminateZeros();
  EQ_abc_rs_2.ApplyMonomialSymmetries();
  EQ_abc_rs_2.SortMonomials();
  EQ_abc_rs_2.ApplyMonomialSymmetriesToContractions();
  EQ_abc_rs_2.RenameDummies();
  EQ_abc_rs_2.SortSummands();
  EQ_abc_rs_2.CollectPrefactors();
  EQ_abc_rs_2.CanonicalisePrefactors();

  // ################################################################################

  Expression ansatz_kinetic_0abcmnpqrs (ansatz_kinetic);

  ansatz_kinetic_0abcmnpqrs.ThreePlusOne(std::vector<Index> ({Index::a}));
  ansatz_kinetic_0abcmnpqrs.SubstituteIndices(Indices ({'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'}), Indices ({'a', 'b', 'c', 'm', 'n', 'p', 'q', 'r', 's'}));

  Expression EQ_abc_rs_3 ( ansatz_kinetic_0abcmnpqrs );
  EQ_abc_rs_3.MultiplyOther(E_mnpqrs);

  EQ_abc_rs_3.EliminateEpsilonEpsilonI();
  EQ_abc_rs_3.EliminateGamma();
  EQ_abc_rs_3.EliminateEpsilon();
  EQ_abc_rs_3.EliminateTracefree();
  EQ_abc_rs_3.EliminateZeros();
  EQ_abc_rs_3.ApplyMonomialSymmetries();
  EQ_abc_rs_3.SortMonomials();
  EQ_abc_rs_3.ApplyMonomialSymmetriesToContractions();
  EQ_abc_rs_3.RenameDummies();
  EQ_abc_rs_3.SortSummands();
  EQ_abc_rs_3.CollectPrefactors();
  EQ_abc_rs_3.CanonicalisePrefactors();

  EQ_abc_rs_1.MultiplyCoefficient(Rational(4, 1));
  EQ_abc_rs_2.MultiplyCoefficient(Rational(4, 1));

  Expression EQ_abc_rs ( EQ_abc_rs_1 );
  EQ_abc_rs.AddOther(EQ_abc_rs_2);
  EQ_abc_rs.AddOther(EQ_abc_rs_3);

  Expression EQ_ab_rs ( EQ_abc_rs );
  EQ_ab_rs.MultiplyOther(epsilon_xbc);

  EQ_ab_rs.EliminateEpsilonEpsilonI();
  EQ_ab_rs.EliminateGamma();
  EQ_ab_rs.EliminateEpsilon();
  EQ_ab_rs.EliminateTracefree();
  EQ_ab_rs.EliminateZeros();
  EQ_ab_rs.ApplyMonomialSymmetries();
  EQ_ab_rs.SortMonomials();
  EQ_ab_rs.ApplyMonomialSymmetriesToContractions();
  EQ_ab_rs.RenameDummies();
  EQ_ab_rs.SortSummandsByLastFactors();
  EQ_ab_rs.CollectPrefactors();
  EQ_ab_rs.CanonicalisePrefactors();

  std::cout << "##########################################" << std::endl;

  std::cout << EQ_ab_rs.GetLatexString() << std::endl;

  return 0;
}
