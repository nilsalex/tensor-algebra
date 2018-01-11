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

static std::ostream& operator<<(std::ostream & stream, mpq_class const & rop) {
  stream << rop.get_str();
  return stream;
}

Expression Expression::NumericSimplify(Indices const & indices, bool print_matrix) const {
  assert(!this->IsZero());

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  std::vector<size_t> numbers (indices.size());

  bool status = true;
  size_t for_pos = indices.size() - 1;
  size_t counter = 0;
  size_t num_combs = pow(4, indices.size());

        ScalarSum sum_tmp = EvaluateIndices(indices, numbers);
        if (!sum_tmp.IsZero()) {
          coefficient_set.merge(sum_tmp.CoefficientSet());
          sum_set.insert(sum_tmp);
        }

  while (status) {
    if (numbers[for_pos] < 3) {
      ++numbers[for_pos];
      {

        ScalarSum sum_tmp = EvaluateIndices(indices, numbers);
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

  std::map<size_t, size_t> coefficient_map;
  std::for_each(coefficient_set.begin(), coefficient_set.end(), [n=0,&coefficient_map](auto a) mutable { coefficient_map[a] = n++; });
  
  std::cout << "number of different (not necessarily linear independent) equations : " << sum_set.size() << std::endl;
  std::cout << "number of coefficients (e_.) in these equations                    : " << coefficient_map.size() << std::endl;
  std::cout << "Thus the problem of finding linear dependencies is equivalent to the problem of finding the null space for a " << sum_set.size() << " by " << coefficient_map.size() << " matrix." << std::endl;

  std::vector<std::vector<Rational>> matrix;
  std::for_each(sum_set.begin(), sum_set.end(), [&coefficient_map, &matrix](auto & a) {
    matrix.push_back(a.CoefficientVector(coefficient_map));
    });

  MatrixXq mq(matrix.size(), coefficient_map.size());

  for (size_t row_counter = 0; row_counter < matrix.size(); ++row_counter) {
    for (size_t column_counter = 0; column_counter < coefficient_map.size(); ++column_counter) {
      Fraction frac = matrix[row_counter][column_counter].get_fraction();
      mq(row_counter, column_counter) = mpq_class(frac.first, frac.second);
    }
  }

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "the rank of the matrix is : " << lu_decompq.rank() << std::endl;
  std::cout << "null space basis: " << std::endl;

  MatrixXq kq = lu_decompq.kernel();

  if (print_matrix) {
    std::cout << kq << std::endl;
  }

  std::set<size_t> coeff_removed; 

  for (size_t column_counter = 0; column_counter < kq.cols(); ++column_counter) {
    for (size_t row_counter = kq.rows() - 1; row_counter >= 0; --row_counter) {
      if (kq(row_counter, column_counter) == 0 ) {
        continue;
      } else if (std::find(coeff_removed.begin(), coeff_removed.end(), row_counter) != coeff_removed.end()) {
        continue;
      } else {
        coeff_removed.insert(row_counter);
        break;
      }
    }
  }

  std::map<size_t, size_t> coefficient_rmap;
  std::for_each(coefficient_map.begin(), coefficient_map.end(), [&coefficient_rmap](auto a) { coefficient_rmap.insert(std::make_pair(a.second, a.first)); });

  Expression ret (*this);
  std::for_each(coeff_removed.begin(), coeff_removed.end(), [&ret, &coefficient_rmap] (auto a) { ret.EliminateVariable(coefficient_rmap.at(a)); });
  ret.CanonicalisePrefactors();
  ret.RedefineScalars();

  return ret;
};
