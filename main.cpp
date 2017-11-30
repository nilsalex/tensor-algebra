#include "Expression.h"
#include "Indices.h"
#include "MonomialExpression.h"
#include "Rational.h"
#include "Tensor.h"
#include "TensorMonomial.h"

#include <iostream>

int main () {
  Tensor eta (2, "eta");
  eta.SetSymmetric();

  Tensor epsilon (3, "epsilon");
  epsilon.SetAntisymmetric();

  TensorMonomial tensor1;
  tensor1.AddFactorRight(eta);
  tensor1.AddFactorRight(epsilon);

  TensorMonomial tensor2;
  tensor2.AddFactorRight(epsilon);
  tensor2.AddFactorRight(eta);

  TensorMonomial tensor3;
  tensor3.AddFactorRight(epsilon);
  tensor3.AddFactorRight(eta);

  Indices indices1 (std::initializer_list<Index>({Index::a, Index::c, Index::b, Index::e, Index::d}));
  Indices indices2 (std::initializer_list<Index>({Index::d, Index::e, Index::b, Index::c, Index::a}));
  Indices indices3 (std::initializer_list<Index>({Index::a, Index::e, Index::b, Index::c, Index::d}));
  Indices indices4 (std::initializer_list<Index>({Index::a, Index::b, Index::c, Index::d, Index::e}));
  Indices indices5 (std::initializer_list<Index>({Index::b, Index::a, Index::c, Index::d, Index::e}));

  MonomialExpression mon_expr1 (tensor1, indices1);
  MonomialExpression mon_expr2 (tensor2, indices2);
  MonomialExpression mon_expr3 (tensor3, indices3);

  auto expr = std::make_shared<Expression>();
  expr->AddSummand(mon_expr1, Rational(5,1), "e");
  expr->AddSummand(mon_expr3, Rational(7,9));
  expr->AddSummand(mon_expr2, Rational(31,6));


  std::cout << "***************************" << std::endl;
  std::cout << "Initial expression:" << std::endl;;
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Applying monomial symmetries:" << std::endl;;
  expr->ApplyMonomialSymmetries();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting monomials:" << std::endl;;
  expr->SortMonomials();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting summands:" << std::endl;;
  expr->SortSummands();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Collecting prefactors:" << std::endl;;
  expr->CollectPrefactors();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "ExchangeSymmetrise {a b c d e} -> {b a c d e}" << std::endl;;
  expr->ExchangeSymmetrise(indices4, indices5);
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Applying monomial symmetries:" << std::endl;;
  expr->ApplyMonomialSymmetries();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting monomials:" << std::endl;;
  expr->SortMonomials();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting summands:" << std::endl;;
  expr->SortSummands();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Collecting prefactors:" << std::endl;;
  expr->CollectPrefactors();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "ExchangeSymmetrise {a b c d e} -> - {b a c d e}" << std::endl;;
  expr->ExchangeSymmetrise(indices4, indices5, true);
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Applying monomial symmetries:" << std::endl;;
  expr->ApplyMonomialSymmetries();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting monomials:" << std::endl;;
  expr->SortMonomials();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Sorting summands:" << std::endl;;
  expr->SortSummands();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  std::cout << "***************************" << std::endl;
  std::cout << "Collecting prefactors:" << std::endl;;
  expr->CollectPrefactors();
  std::cout << "\t" << expr->GetLatexString() << std::endl;

  return 0;
}
