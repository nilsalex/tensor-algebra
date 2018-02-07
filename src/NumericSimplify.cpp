#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include <future>
#include <thread>

#include <Eigen/Dense>

#include <gmpxx.h>

#include "Expression.h"

//int THREADS_NUM = std::thread::hardware_concurrency();
constexpr int THREADS_NUM = 4;

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

static std::ostream& operator<<(std::ostream & stream, mpq_class const & rop) {
  stream << rop.get_str();
  return stream;
}

Expression Expression::NumericSimplify(Indices const & indices, bool print_matrix) const {
  assert(!this->IsZero());

  std::bitset<64> bs(0);
  bs[2 * indices.size()] = true;
  size_t number_of_combinations = bs.to_ulong();

  auto thread_function = [expression=*this,noc=number_of_combinations,indices=indices] (size_t thread_counter, auto coefficient, auto sum) mutable -> void {
    std::set<size_t> coefficient_set;
    std::set<ScalarSum> sum_set;
    for (size_t counter = thread_counter * noc / THREADS_NUM; counter < (thread_counter + 1) * noc / THREADS_NUM; ++counter) {
      std::vector<size_t> numbers (indices.size());
      std::bitset<64> binary_representation(counter);
      for (size_t digit_counter = 0; digit_counter < indices.size(); ++digit_counter) {
        unsigned int binary_digit_a = static_cast<unsigned int>(binary_representation[2 * digit_counter + 1]);
        unsigned int binary_digit_b = static_cast<unsigned int>(binary_representation[2 * digit_counter]);
        *(numbers.rbegin() + digit_counter) = 2 * binary_digit_a + binary_digit_b;
      }

      ScalarSum sum_tmp = expression.EvaluateIndices(indices, numbers);
      if (!sum_tmp.IsZero()) {
        coefficient_set.merge(sum_tmp.CoefficientSet());
        sum_set.insert(sum_tmp);
      }
    }
    coefficient.set_value(coefficient_set);
    sum.set_value(sum_set);
  };

  std::vector<std::thread> t (THREADS_NUM);

  std::vector<std::promise<std::set<size_t>>> coefficients_promises (THREADS_NUM);
  std::vector<std::promise<std::set<ScalarSum>>> sums_promises (THREADS_NUM);

  std::vector<std::future<std::set<size_t>>> coefficients_futures (THREADS_NUM);
  std::vector<std::future<std::set<ScalarSum>>> sums_futures (THREADS_NUM);

  for (int thread_counter = 0; thread_counter < THREADS_NUM; ++thread_counter) {
    coefficients_futures[thread_counter] = coefficients_promises[thread_counter].get_future();
    sums_futures[thread_counter] = sums_promises[thread_counter].get_future();
    std::cout << "Launching thread " << thread_counter << std::endl;
    t[thread_counter] = std::thread(thread_function, thread_counter, std::move(coefficients_promises[thread_counter]),
                                                                     std::move(sums_promises[thread_counter]));
  }

  for (int thread_counter = 0; thread_counter < THREADS_NUM; ++thread_counter) {
    t[thread_counter].join();
    std::cout << "Joined thread " << thread_counter << std::endl;
  }

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  for (int thread_counter = 0; thread_counter < THREADS_NUM; ++thread_counter) {
    coefficient_set.merge(coefficients_futures.at(thread_counter).get());
    sum_set.merge(sums_futures.at(thread_counter).get());
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

  for (int column_counter = 0; column_counter < kq.cols(); ++column_counter) {
    for (int row_counter = kq.rows() - 1; row_counter >= 0; --row_counter) {
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

  return Expression(ret);
}
