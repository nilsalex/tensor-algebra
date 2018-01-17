#include "Expression.h"

#include <iostream>

#include <Eigen/Dense>

#include <gmpxx.h>

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

static std::ostream& operator<<(std::ostream & stream, mpq_class const & rop) {
  stream << rop.get_str();
  return stream;
}

Expression Expression::ApplyGaugeSymmetry(Expression const & delta) const {
  Expression gauge_term = this->MultiplyOther(delta);

  gauge_term.SortMonomials();
  gauge_term.EliminateEtaEta();
  gauge_term.SortMonomials();
  gauge_term.EliminateDelta();
  gauge_term.SortMonomials();
  gauge_term.EliminateEpsilon();

  gauge_term.ApplyMonomialSymmetries();

  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();

  gauge_term.ApplyMonomialSymmetriesToContractions();

  gauge_term.ApplyMonomialSymmetries();
  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();

  gauge_term.EliminateEtaPartial();

  gauge_term.ApplyMonomialSymmetries();
  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();

  gauge_term.RenameDummies();

  gauge_term.ApplyMonomialSymmetries();
  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();

  std::set<ScalarSum> sum_set;
  std::set<size_t> coefficient_set;

  std::for_each(gauge_term.summands->begin(), gauge_term.summands->end(),
    [&coefficient_set,&sum_set] (auto const & a) {
      coefficient_set.merge(a.second->CoefficientSet());
      sum_set.insert(*a.second);
    });

  std::map<size_t, size_t> coefficient_map;
  std::for_each(coefficient_set.begin(), coefficient_set.end(), [n=0,&coefficient_map](auto a) mutable { coefficient_map[a] = n++; });

  std::cout << "number of different (not necessarily linear independent) equations : " << sum_set.size() << std::endl;
  std::cout << "number of coefficients (e_.) in these equations                    : " << coefficient_map.size() << std::endl;

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

  std::cout << mq << std::endl;

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "the rank of the matrix is : " << lu_decompq.rank() << std::endl;
  std::cout << "null space basis: " << std::endl;

  MatrixXq kq = lu_decompq.kernel();

  std::cout << kq << std::endl;

  if (kq.cols() == 0) {
    return gauge_term;
  }

  for (int column_counter = 0; column_counter < kq.cols(); ++column_counter) {
  mpq_class pivot = 0;
    for (int row_counter = 0; row_counter < kq.rows() ; ++row_counter) {
      if (pivot == 0) {
        if (kq(row_counter,column_counter) == 0) {
          continue;
        } else {
          pivot = kq(row_counter, column_counter);
          kq(row_counter, column_counter) = 1;
        }
      } else {
        kq(row_counter, column_counter) = kq(row_counter, column_counter) / pivot;
      }
    }
  }

  std::cout<< "re-ordered:" << std::endl;

  std::cout << kq << std::endl;

  return gauge_term;
}
