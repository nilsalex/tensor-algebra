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
  Expression expr;

  expr.LoadFromFile("simplified_area_mass.prs");

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

  ,expr.GetLatexString());

  expr.ThreePlusOne(std::vector<Index> ({Index::a, Index::c, Index::e, Index::g}));
  expr.SubstituteIndices(Indices ({'b', 'd', 'f', 'h'}), Indices ({'a', 'b', 'm', 'n'}));

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

  ,expr.GetLatexString());

  Tensor A(0, "A", 10);
  Tensor phi1(2, "phi1");
  phi1.SetSymmetric();
  Tensor gamma(2, "gamma");
  gamma.SetSymmetric();

  TensorMonomial tm_gamma_A;
  tm_gamma_A.AddFactorRight(gamma);
  tm_gamma_A.AddFactorRight(A);

  Indices indices_mn {'m', 'n'};

  MonomialExpression me_gamma_A (tm_gamma_A, indices_mn);
  MonomialExpression me_phi1 (phi1, indices_mn);

  Expression E_0m0n;
  E_0m0n.AddSummand(me_gamma_A, Rational(2,1));
  E_0m0n.AddSummand(me_phi1, Rational(-1,1));

  EXPECT_EQ("2 gamma^{ m n } A^{ }\n- 1 phi1^{ m n }", E_0m0n.GetLatexString());

  E_0m0n.SortMonomials();
  E_0m0n.SortSummands();

  EXPECT_EQ("2 gamma^{ m n } A^{ }\n- 1 phi1^{ m n }", E_0m0n.GetLatexString());

  Expression EQ_ab { expr.MultiplyOther(E_0m0n) };

  EQ_ab.EliminateGamma();
  EQ_ab.ApplyMonomialSymmetries();
  EQ_ab.SortMonomials();
  EQ_ab.SortSummands();
  EQ_ab.CollectPrefactors();
  EQ_ab.CanonicalisePrefactors();
  EQ_ab.EliminateZeros();

  EXPECT_EQ("", EQ_ab.GetLatexString());
}
