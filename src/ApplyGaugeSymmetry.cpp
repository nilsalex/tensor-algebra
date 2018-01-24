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
  gauge_term.EliminateEpsilonEpsilonI();
  gauge_term.EliminateEpsilonI();

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
  gauge_term.EliminateEpsilon();

  gauge_term.ApplyMonomialSymmetries();
  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();

  gauge_term.EliminateEtaRankOne();
  gauge_term.RenameDummies();

  gauge_term.ApplyMonomialSymmetries();
  gauge_term.SortMonomials();
  gauge_term.SortSummands();
  gauge_term.CollectPrefactors();
  gauge_term.CanonicalisePrefactors();
  gauge_term.EliminateZeros();
  gauge_term.SortSummandsByPrefactors();

  std::cout << "Gauge transform:" << std::endl;
  std::cout << delta.GetLatexString() << std::endl;

  std::cout << "The gauge term reads:" << std::endl;
  std::cout << gauge_term.GetLatexString() << std::endl;

  std::cout << "It has to vanish identically for each possible xi." << std::endl;

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

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  std::cout << "the rank of the matrix is : " << lu_decompq.rank() << std::endl;
  std::cout << "null space basis: " << std::endl;

  MatrixXq kq = lu_decompq.kernel();

  std::cout << kq << std::endl;

  std::map<size_t, size_t> coefficient_rmap;
  std::for_each(coefficient_map.begin(), coefficient_map.end(), [&coefficient_rmap](auto a) { coefficient_rmap.insert(std::make_pair(a.second, a.first)); });

  std::map<size_t, ScalarSum> replace_map;

  std::vector<std::vector<mpq_class>> kq_vec (kq.rows());
  std::generate(kq_vec.begin(), kq_vec.end(),
    [n=0,&kq] () mutable {
      std::vector<mpq_class> vec_ret (kq.cols());
      std::generate(vec_ret.begin(), vec_ret.end(),
      [m=0,&kq,_n=n++] () mutable {
        return kq(_n,m++);
      });
      return vec_ret;
    });

  std::for_each(kq_vec.cbegin(), kq_vec.cend(),
    [&replace_map,coefficient_rmap,n=0] (auto const & a) mutable {
      ScalarSum scalar_sum;
      std::for_each(a.cbegin(), a.cend(),
        [&scalar_sum, m=0] (auto const & b) mutable {
          scalar_sum.AddScalar(Scalar(Rational(b), ++m));
        });
      replace_map[coefficient_rmap[n++]] = scalar_sum;
    });

  std::cout << "Thus the " << coefficient_map.size() << " original constants can be parameterized as:" << std::endl;
  std::for_each(replace_map.begin(), replace_map.end(),
    [](auto const & a) {
      std::cout << "e_" << a.first << " --> " << a.second.ToString() << std::endl;
    });

  std::cout << "With " << kq.cols() << " remaining constant" << (kq.cols() == 1 ? "" : "(s)") << "." << std::endl;

  Expression expression_copy = *this;

  expression_copy.SubstituteVariables(replace_map);

  expression_copy.CanonicalisePrefactors();
  expression_copy.EliminateZeros();

  return expression_copy;
}
