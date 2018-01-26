#include "Expression.h"
#include "TensorMonomial.h"

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

