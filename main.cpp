#include "Generator.h"
#include "Indices.h"

#include <iostream>

int main () {

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

  std::cout << gtor.Generate().GetLatexString() << std::endl;
/*  
  Indices indices11 {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  Indices indices12 {'b', 'a', 'c', 'd', 'e', 'f', 'g', 'h'};
  Indices indices13 {'a', 'b', 'd', 'c', 'e', 'f', 'g', 'h'};
  Indices indices14 {'a', 'b', 'c', 'd', 'f', 'e', 'g', 'h'};
  Indices indices15 {'a', 'b', 'c', 'd', 'e', 'f', 'h', 'g'};

  Indices indices16 {'c', 'd', 'a', 'b', 'e', 'f', 'g', 'h'};
  Indices indices17 {'a', 'b', 'c', 'd', 'g', 'h', 'e', 'f'};

  Indices indices18 {'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd'};

  Generator gtor2 (indices11, {std::make_pair(indices12, true),
                               std::make_pair(indices13, true),
                               std::make_pair(indices14, true),
                               std::make_pair(indices15, true),
                               std::make_pair(indices16, false),
                               std::make_pair(indices17, false),
                               std::make_pair(indices18, false)});

  std::cout << gtor2.Generate().GetLatexString() << std::endl;
*/
  return 0;
}
