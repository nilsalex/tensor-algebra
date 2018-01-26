#include "Expression.h"
#include "Generator.h"
#include "Indices.h"

#include <fstream>
#include <iostream>

int main() {
  Indices indices { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };

  Generator gtor (indices, { });

  std::cout << "generating ansatz for indices" << std::endl;
  std::cout << indices.ToString() << std::endl;
  std::cout << "without any symmetries" << std::endl << std::endl;

  std::cout << "starting generation ..." << std::endl;
  Expression expr = gtor.Generate();
  std::cout << "completed generation!" << std::endl << std::endl;
  std::cout << "starting numeric simplification ..." << std::endl;
  Expression simplified = expr.NumericSimplify(indices, true);

  simplified.SaveToFile("ansatz_10.prs");

  std::cout << simplified.GetLatexString() << std::endl;

  return 0;
}
