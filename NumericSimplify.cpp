#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include <Eigen/Dense>

#include <gmpxx.h>

#include "Expression.h"

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

std::ostream& operator<<(std::ostream & stream, mpq_class const & rop) {
  stream << rop.get_str();
  return stream;
}

void NumericSimplify(Expression const & expr, Indices const & indices) {
  assert(!expr.IsZero());

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  std::vector<size_t> numbers (indices.size());

  bool status = true;
  size_t for_pos = indices.size() - 1;
  size_t counter = 0;
  size_t num_combs = pow(4, indices.size());

//        std::cout << "Probing index combination " << counter++ << " of " << num_combs << std::endl;
//        std::for_each(numbers.begin(), numbers.end(), [](auto a) { std::cout << a << " "; });
//        std::cout << std::endl;
        ScalarSum sum_tmp = expr.EvaluateIndices(indices, numbers);
        if (!sum_tmp.IsZero()) {
          coefficient_set.merge(sum_tmp.CoefficientSet());
          sum_set.insert(sum_tmp);
        }

  while (status) {
    if (numbers[for_pos] < 3) {
      ++numbers[for_pos];
      {

//        std::cout << "Probing index combination " << counter++ << " of " << num_combs << std::endl;
//        std::for_each(numbers.begin(), numbers.end(), [](auto a) { std::cout << a << " "; });
//        std::cout << std::endl;
        ScalarSum sum_tmp = expr.EvaluateIndices(indices, numbers);
        if (!sum_tmp.IsZero()) {
          coefficient_set.merge(sum_tmp.CoefficientSet());
          sum_set.insert(sum_tmp);
        }

      }
        for_pos = indices.size() - 1;
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

  std::cout << "number of different (not necessarily linear independent) equations : " << sum_set.size() << std::endl;
  std::cout << "number of coefficients (e_.) in these equations                    : " << coefficient_map.size() << std::endl;
  std::cout << "Thus the problem of finding linear dependencies is equivalent to the problem of findinge the null space for a " << sum_set.size() << " by " << coefficient_map.size() << " matrix." << std::endl;

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

  MatrixXq mq(matrix.size(), coefficient_map.size());

  for (unsigned int row_counter = 0; row_counter < matrix.size(); ++row_counter) {
    for (unsigned int column_counter = 0; column_counter < coefficient_map.size(); ++column_counter) {
      Fraction frac = matrix[row_counter][column_counter].get_fraction();
      mq(row_counter, column_counter) = mpq_class(frac.first, frac.second);
    }
  }

/*
  auto svd = Eigen::BDCSVD(m, Eigen::ComputeFullV);

  auto sv = svd.singularValues();
  for (size_t counter = 0; counter < sv.size(); ++counter) {
    std::cout << sv(counter) << std::endl;
  }

  auto V = svd.matrixV();
  std::cout << V << std::endl;
*/

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "the rank of the matrix is : " << lu_decompq.rank() << std::endl;
  std::cout << "null space basis: " << std::endl;

  auto kq = lu_decompq.kernel();

  std::cout << kq << std::endl;
};
