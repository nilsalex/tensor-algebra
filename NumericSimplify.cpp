#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "Expression.h"

void NumericSimplify(Expression const & expr, Indices const & indices) {
  assert(!expr.IsZero());

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  std::vector<size_t> numbers (indices.size());

  bool status = true;
  size_t for_pos = indices.size() - 1;

  while (status) {
    if (numbers[for_pos] < 3) {
      ++numbers[for_pos];
      {

        ScalarSum sum_tmp = expr.EvaluateIndices(indices, numbers);
        coefficient_set.merge(sum_tmp.CoefficientSet());
        sum_set.insert(sum_tmp);

      }
      if (for_pos < indices.size() - 1) {
        ++for_pos;
      }
    } else {
      numbers[for_pos] = 0;
      if (for_pos == 0) {
        status = false;
      } else {
        --for_pos;
      }
    }
  }

  std::for_each(sum_set.begin(), sum_set.end(), [](auto & a) { std::cout << a.ToString() << std::endl; });

  std::for_each(coefficient_set.begin(), coefficient_set.end(), [](auto a) { std::cout << a << " "; });
  std::cout << std::endl;
};
