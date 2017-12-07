#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include <Eigen/Dense>

#include "Expression.h"

void NumericSimplify(Expression const & expr, Indices const & indices) {
  assert(!expr.IsZero());

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  std::vector<size_t> numbers (indices.size());

  bool status = true;
  size_t for_pos = indices.size() - 1;
  size_t counter = 0;
  size_t num_combs = pow(4, indices.size());

  while (status) {
    if (numbers[for_pos] < 3) {
      ++numbers[for_pos];
      {

        ScalarSum sum_tmp = expr.EvaluateIndices(indices, numbers);
        if (!sum_tmp.IsZero()) {
          coefficient_set.merge(sum_tmp.CoefficientSet());
          sum_set.insert(sum_tmp);
        }

        std::cout << "Probing index combination " << counter++ << " of " << num_combs << std::endl;

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

/*
  std::for_each(sum_set.begin(), sum_set.end(), [](auto & a) { std::cout << a.ToString() << std::endl; });
  std::for_each(coefficient_set.begin(), coefficient_set.end(), [](auto a) { std::cout << a << " "; });
  std::cout << std::endl;
*/

  std::map<size_t, size_t> coefficient_map;
  std::for_each(coefficient_set.begin(), coefficient_set.end(), [n=0,&coefficient_map](auto a) mutable { coefficient_map[a] = n++; });

  std::vector<std::vector<Rational>> matrix;
  std::for_each(sum_set.begin(), sum_set.end(), [&coefficient_map, &matrix](auto & a) {
    matrix.push_back(a.CoefficientVector(coefficient_map));
    });

/*
  std::for_each(matrix.begin(), matrix.end(), [](auto & a) {
    std::for_each(a.begin(), a.end(), [](auto & b) {
      std::cout << b.ToString() << "\t";
    });
    std::cout << std::endl;
  });
*/

  auto m = Eigen::MatrixXd(matrix.size(), coefficient_map.size());

  for (unsigned int row_counter = 0; row_counter < matrix.size(); ++row_counter) {
    for (unsigned int column_counter = 0; column_counter < coefficient_map.size(); ++column_counter) {
      Fraction frac = matrix[row_counter][column_counter].get_fraction();
      m(row_counter, column_counter) = frac.first == 0 ? 0. : static_cast<double>(frac.first) / frac.second;
//      std::cout << m(row_counter, column_counter) << "\t";
    }
//    std::cout << std::endl;
  }

  auto svd = Eigen::BDCSVD(m);

  auto sv = svd.singularValues();
  for (size_t counter = 0; counter < sv.size(); ++counter) {
    std::cout << sv(counter) << std::endl;
  }
};
