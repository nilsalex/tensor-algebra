#include "Generator.h"
#include "Indices.h"
#include "NumericSimplify.h"

#include <iostream>

int main () {

/*
  Indices indices1 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices4 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h', 'p', 'q'};
  Indices indices5 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g', 'p', 'q'};

  Indices indices6 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h', 'p', 'q'};
  Indices indices7 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f', 'p', 'q'};

  Indices indices8 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'q', 'p'};

  Indices indices9 {'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'p', 'q'};

  Generator gtor (indices1, {std::make_pair(indices2, true),
                             std::make_pair(indices3, true),
                             std::make_pair(indices4, true),
                             std::make_pair(indices5, true),
                             std::make_pair(indices6, false),
                             std::make_pair(indices7, false),
                             std::make_pair(indices8, false),
                             std::make_pair(indices9, false)});
*/
  

  Indices indices1 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  Indices indices2 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h'};
  Indices indices3 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'};
  Indices indices4 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h'};
  Indices indices5 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'};

  Indices indices6 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h'};
  Indices indices7 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f'};

  Indices indices8 {'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd'};

  Generator gtor (indices1, {std::make_pair(indices2, true),
                               std::make_pair(indices3, true),
                               std::make_pair(indices4, true),
                               std::make_pair(indices5, true),
                               std::make_pair(indices6, false),
                               std::make_pair(indices7, false),
                               std::make_pair(indices8, false)});

/*
  Indices indices1 {'a', 'b', 'c', 'd', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'p', 'q'};
  Indices indices4 {'c', 'd', 'a', 'b', 'p', 'q'};
  Indices indices5 {'a', 'b', 'c', 'd', 'q', 'p'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false),
                             std::make_pair(indices5, false)});
*/
/*
  Indices indices1 {'a', 'b', 'c', 'd', 'p', 'q'};

  Indices indices2 {'b', 'a', 'c', 'd', 'p', 'q'};
  Indices indices3 {'a', 'b', 'd', 'c', 'p', 'q'};
  Indices indices4 {'a', 'b', 'c', 'd', 'q', 'p'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false)});
*/                             
/*
  Indices indices1 {'a', 'b', 'c', 'd'};
  Indices indices2 {'b', 'a', 'c', 'd'};
  Indices indices3 {'a', 'b', 'd', 'c'};
  Indices indices4 {'c', 'd', 'a', 'b'};

  Generator gtor (indices1, {std::make_pair(indices2, false),
                             std::make_pair(indices3, false),
                             std::make_pair(indices4, false)});
*/
/*
  Indices indices {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

  Generator gtor (indices, {});
*/
  Expression expr = gtor.Generate();

  expr.EliminateVariable(6);
  expr.EliminateVariable(8);
  expr.EliminateVariable(9);

  expr.CanonicalisePrefactors();

  std::cout << "Tensor ansatz:" << std::endl;
  std::cout << expr.GetLatexString() << std::endl;

  std::cout << "Generating all linear equations by evaluating indices (this may take some time!) ..." << std::endl;
  NumericSimplify(expr, indices1);

  return 0;
}
