#include "Expression.h"
#include "TensorMonomial.h"

#include <fstream>

#include "gtest/gtest.h"

TEST(Calculations, EpsilonEpsilonIToEta) {
  Tensor epsilon (4, "epsilon");
  Tensor epsilonI (4, "epsilonI");
  Tensor eta (2, "eta");
  Tensor partial (1, "partial");
  Tensor xi (1, "xi");

  epsilon.SetAntisymmetric();
  epsilonI.SetAntisymmetric();

  eta.SetSymmetric();

  TensorMonomial tm;
  tm.AddFactorRight(epsilon);
  tm.AddFactorRight(epsilonI);
  tm.AddFactorRight(eta);
  tm.AddFactorRight(eta);
  tm.AddFactorRight(partial);
  tm.AddFactorRight(xi);

  Indices indices { 'e', 'f', 'g', 'h', 'e', 'f', 'g', 'm', 'a', 'd', 'b', 'c', 'h', 'm' };

  MonomialExpression me (tm, indices);

  Expression expr;
  expr.AddSummand(me, Scalar(Rational(-4, 1), 5));

  EXPECT_EQ("- 4*e_5 epsilon^{ e f g h } epsilonI^{ e f g m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }", expr.GetLatexString());

  expr.SortMonomials();
  EXPECT_EQ("- 4*e_5 epsilon^{ e f g h } epsilonI^{ e f g m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }", expr.GetLatexString());

  expr.EliminateEpsilonEpsilonI();
  EXPECT_EQ("4*e_5 delta^{ e e } delta^{ f f } delta^{ g g } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e e } delta^{ f f } delta^{ g m } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e e } delta^{ f g } delta^{ g f } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e e } delta^{ f g } delta^{ g m } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e e } delta^{ f m } delta^{ g f } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e e } delta^{ f m } delta^{ g g } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e f } delta^{ f e } delta^{ g g } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e f } delta^{ f e } delta^{ g m } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e f } delta^{ f g } delta^{ g e } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e f } delta^{ f g } delta^{ g m } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e f } delta^{ f m } delta^{ g e } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e f } delta^{ f m } delta^{ g g } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e g } delta^{ f e } delta^{ g f } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e g } delta^{ f e } delta^{ g m } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e g } delta^{ f f } delta^{ g e } delta^{ h m } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e g } delta^{ f f } delta^{ g m } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e g } delta^{ f m } delta^{ g e } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e g } delta^{ f m } delta^{ g f } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e m } delta^{ f e } delta^{ g f } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e m } delta^{ f e } delta^{ g g } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e m } delta^{ f f } delta^{ g e } delta^{ h g } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e m } delta^{ f f } delta^{ g g } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n- 4*e_5 delta^{ e m } delta^{ f g } delta^{ g e } delta^{ h f } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }\n+ 4*e_5 delta^{ e m } delta^{ f g } delta^{ g f } delta^{ h e } eta^{ a d } eta^{ b c } partial^{ h } xi^{ m }", expr.GetLatexString());

  expr.EliminateEpsilonI();

  expr.SortMonomials();
  expr.EliminateEtaEta();

  expr.SortMonomials();
  expr.EliminateDelta();

  expr.SortMonomials();
  expr.EliminateEpsilon();

  expr.ApplyMonomialSymmetries();

  expr.SortMonomials();
  expr.SortSummands();
  expr.CollectPrefactors();
  expr.CanonicalisePrefactors();
  expr.EliminateZeros();

  expr.ApplyMonomialSymmetriesToContractions();

  expr.ApplyMonomialSymmetries();
  expr.SortMonomials();
  expr.SortSummands();
  expr.CollectPrefactors();
  expr.CanonicalisePrefactors();
  expr.EliminateZeros();

  expr.EliminateEtaPartial();
  expr.EliminateEpsilon();

  expr.ApplyMonomialSymmetries();
  expr.SortMonomials();
  expr.SortSummands();
  expr.CollectPrefactors();
  expr.CanonicalisePrefactors();
  expr.EliminateZeros();

  expr.EliminateEtaRankOne();
  expr.RenameDummies();

  expr.ApplyMonomialSymmetries();
  expr.SortMonomials();
  expr.SortSummands();
  expr.CollectPrefactors();
  expr.CanonicalisePrefactors();
  expr.EliminateZeros();
  expr.SortSummandsByPrefactors();

  EXPECT_EQ("24*e_5 eta^{ a d } eta^{ b c } partial^{ e } xi^{ e }", expr.GetLatexString());
}

TEST(Calculations, 3plus1) {
  Expression ansatz;

  ansatz.LoadFromFile("simplified_area_mass.prs");

  EXPECT_EQ(

"1*e_1 eta^{ a c } eta^{ b d } eta^{ e h } eta^{ f g }\n\
+ 1*e_1 eta^{ a d } eta^{ b c } eta^{ e g } eta^{ f h }\n\
- 1*e_1 eta^{ a c } eta^{ b d } eta^{ e g } eta^{ f h }\n\
- 1*e_1 eta^{ a d } eta^{ b c } eta^{ e h } eta^{ f g }\n\
+ 1*e_2 eta^{ a c } eta^{ b e } eta^{ d h } eta^{ f g }\n\
+ 1*e_2 eta^{ a c } eta^{ b f } eta^{ d g } eta^{ e h }\n\
+ 1*e_2 eta^{ a c } eta^{ b g } eta^{ d f } eta^{ e h }\n\
+ 1*e_2 eta^{ a c } eta^{ b h } eta^{ d e } eta^{ f g }\n\
+ 1*e_2 eta^{ a d } eta^{ b e } eta^{ c g } eta^{ f h }\n\
+ 1*e_2 eta^{ a d } eta^{ b f } eta^{ c h } eta^{ e g }\n\
+ 1*e_2 eta^{ a d } eta^{ b g } eta^{ c e } eta^{ f h }\n\
+ 1*e_2 eta^{ a d } eta^{ b h } eta^{ c f } eta^{ e g }\n\
+ 1*e_2 eta^{ a e } eta^{ b c } eta^{ d g } eta^{ f h }\n\
+ 1*e_2 eta^{ a e } eta^{ b d } eta^{ c h } eta^{ f g }\n\
+ 1*e_2 eta^{ a f } eta^{ b c } eta^{ d h } eta^{ e g }\n\
+ 1*e_2 eta^{ a f } eta^{ b d } eta^{ c g } eta^{ e h }\n\
+ 1*e_2 eta^{ a g } eta^{ b c } eta^{ d e } eta^{ f h }\n\
+ 1*e_2 eta^{ a g } eta^{ b d } eta^{ c f } eta^{ e h }\n\
+ 1*e_2 eta^{ a h } eta^{ b c } eta^{ d f } eta^{ e g }\n\
+ 1*e_2 eta^{ a h } eta^{ b d } eta^{ c e } eta^{ f g }\n\
- 1*e_2 eta^{ a c } eta^{ b e } eta^{ d g } eta^{ f h }\n\
- 1*e_2 eta^{ a c } eta^{ b f } eta^{ d h } eta^{ e g }\n\
- 1*e_2 eta^{ a c } eta^{ b g } eta^{ d e } eta^{ f h }\n\
- 1*e_2 eta^{ a c } eta^{ b h } eta^{ d f } eta^{ e g }\n\
- 1*e_2 eta^{ a d } eta^{ b e } eta^{ c h } eta^{ f g }\n\
- 1*e_2 eta^{ a d } eta^{ b f } eta^{ c g } eta^{ e h }\n\
- 1*e_2 eta^{ a d } eta^{ b g } eta^{ c f } eta^{ e h }\n\
- 1*e_2 eta^{ a d } eta^{ b h } eta^{ c e } eta^{ f g }\n\
- 1*e_2 eta^{ a e } eta^{ b c } eta^{ d h } eta^{ f g }\n\
- 1*e_2 eta^{ a e } eta^{ b d } eta^{ c g } eta^{ f h }\n\
- 1*e_2 eta^{ a f } eta^{ b c } eta^{ d g } eta^{ e h }\n\
- 1*e_2 eta^{ a f } eta^{ b d } eta^{ c h } eta^{ e g }\n\
- 1*e_2 eta^{ a g } eta^{ b c } eta^{ d f } eta^{ e h }\n\
- 1*e_2 eta^{ a g } eta^{ b d } eta^{ c e } eta^{ f h }\n\
- 1*e_2 eta^{ a h } eta^{ b c } eta^{ d e } eta^{ f g }\n\
- 1*e_2 eta^{ a h } eta^{ b d } eta^{ c f } eta^{ e g }\n\
+ 1*e_3 eta^{ a e } eta^{ b f } eta^{ c h } eta^{ d g }\n\
+ 1*e_3 eta^{ a f } eta^{ b e } eta^{ c g } eta^{ d h }\n\
+ 1*e_3 eta^{ a g } eta^{ b h } eta^{ c f } eta^{ d e }\n\
+ 1*e_3 eta^{ a h } eta^{ b g } eta^{ c e } eta^{ d f }\n\
- 1*e_3 eta^{ a e } eta^{ b f } eta^{ c g } eta^{ d h }\n\
- 1*e_3 eta^{ a f } eta^{ b e } eta^{ c h } eta^{ d g }\n\
- 1*e_3 eta^{ a g } eta^{ b h } eta^{ c e } eta^{ d f }\n\
- 1*e_3 eta^{ a h } eta^{ b g } eta^{ c f } eta^{ d e }\n\
+ 1*e_4 eta^{ a e } eta^{ b g } eta^{ c h } eta^{ d f }\n\
+ 1*e_4 eta^{ a e } eta^{ b h } eta^{ c f } eta^{ d g }\n\
+ 1*e_4 eta^{ a f } eta^{ b g } eta^{ c e } eta^{ d h }\n\
+ 1*e_4 eta^{ a f } eta^{ b h } eta^{ c g } eta^{ d e }\n\
+ 1*e_4 eta^{ a g } eta^{ b e } eta^{ c f } eta^{ d h }\n\
+ 1*e_4 eta^{ a g } eta^{ b f } eta^{ c h } eta^{ d e }\n\
+ 1*e_4 eta^{ a h } eta^{ b e } eta^{ c g } eta^{ d f }\n\
+ 1*e_4 eta^{ a h } eta^{ b f } eta^{ c e } eta^{ d g }\n\
- 1*e_4 eta^{ a e } eta^{ b g } eta^{ c f } eta^{ d h }\n\
- 1*e_4 eta^{ a e } eta^{ b h } eta^{ c g } eta^{ d f }\n\
- 1*e_4 eta^{ a f } eta^{ b g } eta^{ c h } eta^{ d e }\n\
- 1*e_4 eta^{ a f } eta^{ b h } eta^{ c e } eta^{ d g }\n\
- 1*e_4 eta^{ a g } eta^{ b e } eta^{ c h } eta^{ d f }\n\
- 1*e_4 eta^{ a g } eta^{ b f } eta^{ c e } eta^{ d h }\n\
- 1*e_4 eta^{ a h } eta^{ b e } eta^{ c f } eta^{ d g }\n\
- 1*e_4 eta^{ a h } eta^{ b f } eta^{ c g } eta^{ d e }\n\
+ 1*e_5 epsilon^{ a b c d } eta^{ e h } eta^{ f g }\n\
+ 1*e_5 epsilon^{ e f g h } eta^{ a d } eta^{ b c }\n\
- 1*e_5 epsilon^{ a b c d } eta^{ e g } eta^{ f h }\n\
- 1*e_5 epsilon^{ e f g h } eta^{ a c } eta^{ b d }\n\
+ 1*e_6 epsilon^{ a b e f } eta^{ c h } eta^{ d g }\n\
+ 1*e_6 epsilon^{ a b g h } eta^{ c f } eta^{ d e }\n\
+ 1*e_6 epsilon^{ c d e f } eta^{ a h } eta^{ b g }\n\
+ 1*e_6 epsilon^{ c d g h } eta^{ a f } eta^{ b e }\n\
- 1*e_6 epsilon^{ a b e f } eta^{ c g } eta^{ d h }\n\
- 1*e_6 epsilon^{ a b g h } eta^{ c e } eta^{ d f }\n\
- 1*e_6 epsilon^{ c d e f } eta^{ a g } eta^{ b h }\n\
- 1*e_6 epsilon^{ c d g h } eta^{ a e } eta^{ b f }"

  ,ansatz.GetLatexString());

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

  MonomialExpression me_gamma_A (TensorMonomial ({gamma, A}), Indices({'m', 'n'}));
  MonomialExpression me_phi1 (phi1, Indices({'m', 'n'}));

  MonomialExpression me_gamma_B (TensorMonomial ({gamma, B}), Indices({'m', 'n', 'p'}));
  MonomialExpression me_gamma_phi1_epsilon (TensorMonomial ({gamma, phi1, epsilon}), Indices({'u', 'v', 'u', 'v', 'm', 'n', 'p'}));
  MonomialExpression me_A_epsilon (TensorMonomial ({A, epsilon}), Indices({'m', 'n', 'p'}));
  MonomialExpression me_phi3_epsilon (TensorMonomial ({phi3, epsilon}), Indices({'m', 'u', 'n', 'p', 'u'}));

  MonomialExpression me_B_epsilon (TensorMonomial ({B, epsilon}), Indices({'m', 'n', 'p', 'q'}));
  MonomialExpression me_gamma_phi1_gamma_gamma (TensorMonomial ({gamma, phi1, gamma, gamma}), Indices ({'u', 'v', 'u', 'v', 'm', 'p', 'q', 'n'}));
  MonomialExpression me_epsilon_epsilon_phi2 (TensorMonomial ({epsilon, epsilon, phi2}), Indices ({'m', 'n', 'u', 'p', 'q', 'v', 'u', 'v'}));

  Expression E_0m0n;
  E_0m0n.AddSummand(me_gamma_A, Rational(2,1));
  E_0m0n.AddSummand(me_phi1, Rational(-1,1));

  Expression E_0mnp;
  E_0mnp.AddSummand(me_gamma_B, Rational(-1, 1));
  me_gamma_B.ExchangeIndices(Indices({'m', 'n', 'p'}), Indices({'m', 'p', 'n'}));
  E_0mnp.AddSummand(me_gamma_B, Rational(1, 1));
  E_0mnp.AddSummand(me_gamma_phi1_epsilon, Rational(1, 2));
  E_0mnp.AddSummand(me_A_epsilon, Rational(-1, 1));
  E_0mnp.AddSummand(me_phi3_epsilon, Rational(1,1));

  Expression E_mnpq;
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

  EXPECT_EQ(
"2 gamma^{ m n } A^{ }\n\
- 1 phi1^{ m n }"
  , E_0m0n.GetLatexString());

  EXPECT_EQ(
"- 1 gamma^{ m n } B^{ p }\n\
+ 1 gamma^{ m p } B^{ n }\n\
+ 1/2 gamma^{ u v } phi1^{ u v } epsilon^{ m n p }\n\
- 1 A^{ } epsilon^{ m n p }\n\
+ 1 phi3^{ m u } epsilon^{ n p u }"
  , E_0mnp.GetLatexString());

  EXPECT_EQ(
"- 1 B^{ m } epsilon^{ n p q }\n\
+ 1 B^{ n } epsilon^{ m p q }\n\
- 1 B^{ p } epsilon^{ q m n }\n\
+ 1 B^{ q } epsilon^{ p m n }\n\
+ 1 gamma^{ u v } phi1^{ u v } gamma^{ m p } gamma^{ q n }\n\
- 1 gamma^{ u v } phi1^{ u v } gamma^{ m q } gamma^{ p n }\n\
+ 1 epsilon^{ m n u } epsilon^{ p q v } phi2^{ u v }"
  , E_mnpq.GetLatexString());

  Expression ansatz_0m0n (ansatz);

  ansatz_0m0n.ThreePlusOne(std::vector<Index> ({Index::a, Index::c, Index::e, Index::g}));
  ansatz_0m0n.SubstituteIndices(Indices ({'b', 'd', 'f', 'h'}), Indices ({'a', 'b', 'm', 'n'}));

  EXPECT_EQ(

"- 1*e_1 gamma^{ a b } gamma^{ m n }\n\
- 1*e_2 gamma^{ a m } gamma^{ b n }\n\
- 1*e_2 gamma^{ a n } gamma^{ b m }\n\
- 1*e_2 gamma^{ a b } gamma^{ m n }\n\
- 1*e_2 gamma^{ a b } gamma^{ m n }\n\
- 1*e_3 gamma^{ a m } gamma^{ b n }\n\
- 1*e_3 gamma^{ a n } gamma^{ b m }\n\
- 1*e_4 gamma^{ a n } gamma^{ b m }\n\
- 1*e_4 gamma^{ a m } gamma^{ b n }"

  ,ansatz_0m0n.GetLatexString());

  Expression EQ_ab_1 (ansatz_0m0n);
  EQ_ab_1.MultiplyOther(E_0m0n);

  EQ_ab_1.EliminateGamma();
  EQ_ab_1.RenameDummies();
  EQ_ab_1.ApplyMonomialSymmetries();
  EQ_ab_1.SortMonomials();
  EQ_ab_1.SortSummands();
  EQ_ab_1.CollectPrefactors();
  EQ_ab_1.CanonicalisePrefactors();
  EQ_ab_1.EliminateZeros();

  EXPECT_EQ(

"(1*e_1 + 2*e_2) gamma^{ a b } phi1^{ c c }\n\
+ (- 6*e_1 - 16*e_2 - 4*e_3 - 4*e_4) gamma^{ a b } A^{ }\n\
+ (2*e_2 + 2*e_3 + 2*e_4) phi1^{ a b }"

  , EQ_ab_1.GetLatexString());


  Expression ansatz_0mnp (ansatz);

  ansatz_0mnp.ThreePlusOne(std::vector<Index> ({Index::a, Index::c, Index::e}));
  ansatz_0mnp.SubstituteIndices(Indices ({'b', 'd', 'f', 'g', 'h'}), Indices ({'a', 'b', 'm', 'n', 'p'}));

  EXPECT_EQ(
"- 1*e_5 epsilon^{ m n p } gamma^{ a b }\n\
- 1*e_6 epsilon^{ a n p } gamma^{ b m }\n\
- 1*e_6 epsilon^{ b n p } gamma^{ a m }"
  , ansatz_0mnp.GetLatexString());

  Expression EQ_ab_2 (ansatz_0mnp);
  EQ_ab_2.MultiplyOther(E_0mnp);

  EQ_ab_2.EliminateEpsilonEpsilonI();
  EQ_ab_2.EliminateEpsilon();
  EQ_ab_2.EliminateGamma();
  EQ_ab_2.EliminateTracefree();
  EQ_ab_2.ApplyMonomialSymmetries();
  EQ_ab_2.SortMonomials();
  EQ_ab_2.ApplyMonomialSymmetriesToContractions();
  EQ_ab_2.RenameDummies();
  EQ_ab_2.SortSummands();
  EQ_ab_2.CollectPrefactors();
  EQ_ab_2.CanonicalisePrefactors();
  EQ_ab_2.EliminateZeros();

  EXPECT_EQ(
"(- 3*e_5 - 2*e_6) gamma^{ a b } phi1^{ c c }\n\
+ (6*e_5 + 4*e_6) gamma^{ a b } A^{ }\n\
- 4*e_6 phi3^{ a b }"
  , EQ_ab_2.GetLatexString());

  Expression ansatz_mnpq (ansatz);

  ansatz_mnpq.ThreePlusOne(std::vector<Index> ({Index::a, Index::c}));
  ansatz_mnpq.SubstituteIndices(Indices ({'b', 'd', 'e', 'f', 'g', 'h'}), Indices ({'a', 'b', 'm', 'n', 'p', 'q'}));

  Expression EQ_ab_3 (ansatz_mnpq);
  EQ_ab_3.MultiplyOther(E_mnpq);

  EQ_ab_3.EliminateEpsilonEpsilonI();
  EQ_ab_3.EliminateGamma();
  EQ_ab_3.EliminateEpsilon();
  EQ_ab_3.EliminateTracefree();
  EQ_ab_3.EliminateZeros();
  EQ_ab_3.ApplyMonomialSymmetries();
  EQ_ab_3.SortMonomials();
  EQ_ab_3.ApplyMonomialSymmetriesToContractions();
  EQ_ab_3.RenameDummies();
  EQ_ab_3.SortSummands();
  EQ_ab_3.CollectPrefactors();
  EQ_ab_3.CanonicalisePrefactors();

  EXPECT_EQ(
"(12*e_1 + 16*e_2) gamma^{ a b } phi1^{ c c }\n\
+ (4*e_1 + 8*e_2) gamma^{ a b } phi2^{ c c }\n\
- 8*e_2 phi2^{ a b }"
  , EQ_ab_3.GetLatexString());

  EQ_ab_1.MultiplyCoefficient(Rational(4, 1));
  EQ_ab_2.MultiplyCoefficient(Rational(4, 1));

  Expression EQ_ab (EQ_ab_1);
  EQ_ab.AddOther(EQ_ab_2);
  EQ_ab.AddOther(EQ_ab_3);

  EQ_ab.SortSummands();
  EQ_ab.CollectPrefactors();
  EQ_ab.CanonicalisePrefactors();
  EQ_ab.EliminateZeros();

  EXPECT_EQ(
"(16*e_1 + 24*e_2 - 12*e_5 - 8*e_6) gamma^{ a b } phi1^{ c c }\n\
+ (4*e_1 + 8*e_2) gamma^{ a b } phi2^{ c c }\n\
+ (- 24*e_1 - 64*e_2 - 16*e_3 - 16*e_4 + 24*e_5 + 16*e_6) gamma^{ a b } A^{ }\n\
+ (8*e_2 + 8*e_3 + 8*e_4) phi1^{ a b }\n\
- 8*e_2 phi2^{ a b }\n\
- 16*e_6 phi3^{ a b }"
  ,EQ_ab.GetLatexString());
}
