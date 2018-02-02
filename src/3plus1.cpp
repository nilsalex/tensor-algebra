#include "Expression.h"

#include <fstream>
#include <iostream>

int main() {
  Expression expr;

  expr.LoadFromFile("simplified_area_mass.prs");

  std::cout << expr.GetLatexString() << std::endl;

  expr.ThreePlusOne(std::vector<Index> ({Index::a, Index::c, Index::e, Index::g}));

  expr.ApplyMonomialSymmetries();
  expr.SortMonomials();
  expr.SortSummands();
  expr.CollectPrefactors();
  expr.SortSummandsByPrefactors();

  return 0;
}
