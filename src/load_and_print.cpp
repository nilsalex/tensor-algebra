#include <iostream>
#include <fstream>

#include "Expression.h"

int main () {
  Expression expr;
  expr.LoadFromFile("simplified_area_kinetic.prs");
  std::cout << expr.GetLatexString() << std::endl;
  return 0;
}
