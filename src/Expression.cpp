#include <algorithm>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <Eigen/Dense>
#include "gmpxx.h"

#include "Expression.h"
#include "MonomialExpression.h"
#include "Scalar.h"
#include "ScalarSum.h"
#include "TensorMonomial.h"

template <typename Archive>
void Expression::serialize(Archive & ar, unsigned int const version) {
  if (version > 0) {
  } else {
  }
  ar & dimension;
  ar & summands;
}

template void Expression::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int const);
template void Expression::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, unsigned int const);

typedef Eigen::Matrix<mpq_class, Eigen::Dynamic, Eigen::Dynamic> MatrixXq;

Expression::Expression() : dimension(4), summands(std::make_unique<std::vector<Summand>>()) { }

Expression::Expression(Expression const & other) : dimension(other.dimension), summands(std::make_unique<Sum>()) {
  std::for_each(other.summands->cbegin(), other.summands->cend(), [this](auto const & it) {
    summands->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
    auto monexp_new = std::make_unique<MonomialExpression>(*(it.first));
    auto scalar_new = std::make_unique<ScalarSum>(*(it.second));
    std::swap((*(--summands->end())).first, monexp_new);
    std::swap((*(--summands->end())).second, scalar_new);
  });
}

Expression & Expression::operator=(Expression const & other) {
  Expression tmp ( other );
  dimension = other.dimension;
  std::swap(summands, tmp.summands);
  return *this;
}

void Expression::set_dimension (unsigned int const dimension_set) {
  dimension = dimension_set;
}

unsigned int Expression::get_dimension (void) const {
  return dimension;
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, ScalarSum const & scalar_sum) {
  summands->push_back(std::make_pair(std::make_unique<MonomialExpression>(monomial_expression), std::make_unique<ScalarSum>(scalar_sum)));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar) {
  summands->push_back(std::make_pair(std::make_unique<MonomialExpression>(monomial_expression), std::make_unique<ScalarSum>(scalar)));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression) {
  AddSummand(monomial_expression, Scalar());
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, Rational const & rational) {
  AddSummand(monomial_expression, Scalar(rational));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, size_t variable) {
  AddSummand(monomial_expression, Scalar(variable));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, size_t variable) {
  AddSummand(monomial_expression, Scalar(rational, variable));
}

std::set<size_t> Expression::GetVariableSet () const {
  std::set<size_t> ret;
  std::for_each(summands->cbegin(), summands->cend(),
    [&ret] (auto const & summand) {
      ret.merge(summand.second->CoefficientSet());
    });

  return ret;
}

std::map<size_t, size_t> Expression::GetCoefficientMap () const {
  std::set<size_t> variables = GetVariableSet ();

  std::map<size_t, size_t> coefficient_map;
  std::for_each(variables.cbegin(), variables.cend(), [&coefficient_map,n=0] (auto const & a) mutable { coefficient_map[a] = n++; });

  return coefficient_map;
}

std::vector<std::vector<Rational>> Expression::GetPrefactorMatrix () const {
  std::map<size_t, size_t> coefficient_map = GetCoefficientMap ();

  std::vector<std::vector<Rational>> prefactor_matrix;
  std::for_each(summands->cbegin(), summands->cend(),
    [&prefactor_matrix, &coefficient_map] (auto const & summand) {
      prefactor_matrix.push_back(summand.second->CoefficientVector(coefficient_map));
    });

  std::sort(prefactor_matrix.begin(), prefactor_matrix.end());
  prefactor_matrix.erase(std::unique(prefactor_matrix.begin(), prefactor_matrix.end()), prefactor_matrix.end());

  return prefactor_matrix;
}

void Expression::SortMonomials() {
  auto summands_new = std::make_unique<Sum>();
  std::transform(summands->cbegin(), summands->cend(), std::back_inserter(*summands_new), [](auto const & a) { auto ret = std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second)); ret.first->Sort(); return ret; });
  std::swap(summands, summands_new);
}

void Expression::ApplyMonomialSymmetries() {
  for (auto & summand : *summands) {
    bool even = summand.first->ApplySymmetries();
    if (!even) {
      summand.second->Negate();
    }
  }
}

void Expression::ApplyMonomialSymmetriesToContractions() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->ApplySymmetriesToContractions();
        if (ret == SYM_EVEN) {
          repeat = true;
        } else if (ret == SYM_ODD) {
          a.second->Negate();
          repeat = true;
        } else if (ret != NO_ACTION) {
          assert(false);
        }
      });
  }
}

void Expression::CollectPrefactors() {
  if (summands->size() < 2) {
    return;
  }

  auto summands_new = std::make_unique<std::vector<std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>>>();

  auto last = summands->begin();
  for (auto it = summands->begin() + 1; it < summands->end(); ++it) {
    if (*(last->first) == *(it->first)) {
      last->second->MergeWithOther(*(it->second));
      if (std::distance(it, summands->end()) == 1) {
        summands_new->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
        std::swap(*(--(summands_new->end())), *last);
      }
    } else {
        summands_new->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
        std::swap(*(--(summands_new->end())), *last);
      if (std::distance(it, summands->end()) == 1) {
        summands_new->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
        std::swap(*(--(summands_new->end())), *it);
      } else {
        last = it;
      }
    }
  }

  std::swap(summands, summands_new);

  CanonicalisePrefactors();
}

void Expression::CanonicalisePrefactors() {
  std::for_each(summands->begin(), summands->end(), [](auto & a) { a.second->Sort(); a.second->Collect(); });

  EliminateZeros();
}

void Expression::EliminateGamma() {
  bool repeat = true;

  while (repeat) {
    auto summands_new = std::make_unique<Sum>();
    repeat = false;
    std::transform(summands->cbegin(), summands->cend(), std::back_inserter(*summands_new),
      [&repeat](auto const & a) {
        auto summand_new = std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second));;
        Status ret = summand_new.first->EliminateGamma();
        if (ret == DELTA_OK ) {
          repeat = true;
        } else if (ret == DELTA_TO_TRACE) {
          summand_new.second->MultiplyCoefficient(Rational(3, 1));
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
        return summand_new;
      });
    std::swap(summands, summands_new);
  }

}

void Expression::EliminateDelta() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(), 
      [&repeat,this](auto & a) {
        Status ret = a.first->EliminateDelta();
        if (ret == DELTA_OK ) {
          repeat = true;
        } else if (ret == DELTA_TO_TRACE) {
          a.second->MultiplyCoefficient(Rational(dimension, 1));
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
      });
  }

}

void Expression::EliminateEpsilon() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat,this](auto & a) {
        Status ret = a.first->EliminateEpsilon(dimension);
        if (ret == EPSILON_TO_ZERO) {
          a.second->MultiplyCoefficient(Rational(0, 1));
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
      });
    EliminateZeros();
  }
}

void Expression::EliminateEpsilonI() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateEpsilonI();
        if (ret == EPSILONI) {
          repeat = true;
        } else if (ret != NO_ACTION) {
          assert(false);
        }
      });
    EliminateZeros();
  }
}

static bool Sort(std::vector<size_t> vec) {
  if (vec.empty()) {
    return true;
  }
  unsigned int swap_counter = 0;
  bool swapped;
  do {
    swapped = false;
    for (size_t counter = 0; counter < vec.size() - 1; ++counter) {
      if (vec[counter] > vec[counter + 1]) {
        std::swap(vec[counter], vec[counter + 1]);
        swapped = true;
        swap_counter += 1;
      }
    }
  } while (swapped);
  return (swap_counter % 2 == 0) ? true : false; 
}

void Expression::EliminateEpsilonEpsilonI() {
  bool repeat = true;

  while (repeat) {
    // new vector of summands
    auto summands_new = std::make_unique<Sum>();
    // don't repeat unless there has been some action
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat,&summands_new,this](auto const & a) {
        // eliminate epsilon and epsilonI in summand
        // return indices of epsilon and epsilonI
        auto indices_pair = a.first->EliminateEpsilonEpsilonI(dimension);   

        // if there was no action (returned indices are of size zero),
        // push back old summand to new vector of summands
        //
        // else, for every permutation, push back the (dimension) deltas and
        // then what remains from the original summand (if there is any tensor left)
        if ( indices_pair.first.size() == 0 && indices_pair.second.size() == 0 && !a.first->IsZero()) {
          summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second)));
        } else if ( indices_pair.first.size() == dimension && indices_pair.second.size() == dimension ) {
          Tensor delta (2, (dimension == 3 ? "gamma" : "delta"));
          delta.SetSymmetric();

          TensorMonomial dim_deltas;

          std::vector<size_t> index_numbers (dimension);
          std::generate(index_numbers.begin(), index_numbers.end(),
            [&delta,&dim_deltas,n=0] () mutable {
              dim_deltas.AddFactorRight(delta);
              return n++;
            });

          do {
            auto index_number_copy = index_numbers;
            auto const index_number_copy_const = index_numbers;
            bool sign = Sort(index_number_copy);

            ScalarSum coeff = *a.second;
            if (dimension == 4) {
              if (sign) {
                coeff.Negate(); // Attention! Additional minus sign because epsilon epsilonI = - delta delta delta delta
              }
            } else if (dimension == 3) {
              if (!sign) {
                coeff.Negate();
              }
            } else {
              assert(false);
            }

            std::vector<Index> dim_deltas_indices (2*dimension);

            std::for_each(index_number_copy_const.cbegin(), index_number_copy_const.cend(),
              [&dim_deltas_indices,&indices_pair,&index_numbers] (auto const & a) {
                dim_deltas_indices[2*a] = indices_pair.first.at(a);
                dim_deltas_indices[2*a+1] = indices_pair.second.at(index_numbers.at(a));
              });
            
            MonomialExpression dim_deltas_me (dim_deltas, Indices(dim_deltas_indices));
            MonomialExpression new_summand = dim_deltas_me.MultiplyOther(*a.first);

            summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(new_summand), std::make_unique<ScalarSum>(coeff)));
          } while (std::next_permutation(index_numbers.begin(), index_numbers.end()));

//          repeat = true;

        } else {
          assert(false);
        }
      });
    std::swap(summands, summands_new);
  }
}

void Expression::EliminateTracefree() {
  std::for_each(summands->begin(), summands->end(),
    [](auto & a) {
      if (a.first->EliminateTracefree() == TRACE_TO_ZERO) {
        a.second->MultiplyCoefficient(Rational(0, 1));
      }
    });

  EliminateZeros();
}

void Expression::EliminateGammaGamma() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateGammaGamma();
        if (ret == GAMMA_GAMMA_TO_TRACE) {
          a.second->MultiplyCoefficient(Rational(3, 1));
          repeat = true;
        } else if (ret == GAMMA_GAMMA_TO_DELTA) {
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
      });
  }
}

void Expression::EliminateEtaEta() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateEtaEta();
        if (ret == ETA_ETA_TO_TRACE) {
          a.second->MultiplyCoefficient(Rational(4, 1));
          repeat = true;
        } else if (ret == ETA_ETA_TO_DELTA) {
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
      });
  }
}

void Expression::EliminateEtaPartial() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateEtaPartial();
        if (ret == ETA_PARTIAL_CONTRACTION) {
          repeat = true;
        } else if (ret != NO_ACTION) {
          assert(false);
        }
      });
  }
}

void Expression::EliminateEtaRankOne() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateEtaRankOne();
        if (ret == ETA_CONTRACTION) {
          repeat = true;
        } else if (ret != NO_ACTION) {
          assert(false);
        }
      });
  }
}

void Expression::EliminateZeros() {
  auto summands_new = std::make_unique<std::vector<std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>>>();

  for (auto & summand : *summands) {
    if (summand.second->IsZero()) {
      continue;
    }
    summands_new->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
    std::swap(*(--(summands_new->end())), summand);
  }

  std::swap(summands, summands_new);
}

void Expression::SubstituteIndices(Indices const & indices_old, Indices const & indices_new) {
  std::for_each(summands->begin(), summands->end(),
    [&indices_old,&indices_new] (auto & a) {
      a.first->ExchangeIndices(indices_old,indices_new);
    });
}

void Expression::SubstituteFreeIndices(Indices const & indices_old, Indices const & indices_new) {
  std::for_each(summands->begin(), summands->end(),
    [&indices_old,&indices_new] (auto & a) {
      a.first->ExchangeFreeIndices(indices_old,indices_new);
    });
}

void Expression::SubstituteVariable(size_t const variable_old, ScalarSum const & scalar_sum_new) {
  std::for_each(summands->begin(), summands->end(),
    [variable_old,scalar_sum_new](auto & a) {
      a.second->SubstituteVariable(variable_old, scalar_sum_new);
    });
}

void Expression::SubstituteVariables(std::map<size_t, ScalarSum> substitution_map) {
  std::for_each(summands->begin(), summands->end(),
    [substitution_map](auto & a) {
      a.second->SubstituteVariables(substitution_map);
    });
}

void Expression::SubstituteVariables(std::map<size_t, size_t> substitution_map) {
  std::for_each(summands->begin(), summands->end(),
    [substitution_map](auto & a) {
      a.second->SubstituteVariables(substitution_map);
    });
}

void Expression::EliminateVariable(size_t const variable) {
  std::for_each(summands->begin(), summands->end(), [variable](auto & a) { a.second->EliminateVariable(variable); });
}

void Expression::SortSummands() {
  std::sort(summands->begin(), summands->end());
}

bool CompareSummandsReversed(Summand const & a, Summand const & b);

void Expression::SortSummandsByLastFactors() {
  std::sort(summands->begin(), summands->end(), CompareSummandsReversed);
}

void Expression::ExchangeSymmetrise(Indices const & indices1, Indices const & indices2, bool anti) {
  auto summands_new = std::make_unique<Sum>();
  
  for (auto & summand : *summands) {
    summand.second->DivideByTwo();
    Summand summand_copy;
    summand_copy.first = std::make_unique<MonomialExpression>(*summand.first);
    summand_copy.second = std::make_unique<ScalarSum>(*summand.second);
    summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>()));
    std::swap(*(--(summands_new->end())), summand);
    if (anti) {
      summand_copy.second->Negate();
    }
    summand_copy.first->ExchangeFreeIndices(indices1, indices2);
    summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>()));
    std::swap(*(--(summands_new->end())), summand_copy);
  }
  
  std::swap(summands, summands_new);
}

void Expression::SortSummandsByPrefactors() {
  std::sort(summands->begin(), summands->end(), [](auto & a, auto & b) {
    if (*a.second < *b.second) {
      return true;
    } else if (*b.second < *a.second) {
      return false;
    } else if (*a.first < *b.first) {
      return true;
    } else {
      return false;
    }
  });
}

void Expression::ThreePlusOne(std::vector<Index> indices_to_zero) {
  auto summands_new = std::make_unique<Sum>();

  std::for_each(summands->begin(), summands->end(),
    [&indices_to_zero,&summands_new] (auto & a) { 
      auto status = a.first->ThreePlusOne(indices_to_zero);
      if (status == ZERO_ZERO) {
        return;
      } else if (status == ZERO_POSITIVE) {
        summands_new->emplace_back(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second));
      } else if (status == ZERO_NEGATIVE) {
        summands_new->emplace_back(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second));
        summands_new->back().second->Negate();
      } else {
        assert(false);
      }
    });

  dimension = 3;
  std::swap(summands_new, summands);
}

bool Expression::ContainsMonomial (MonomialExpression const & monexpr) const {
  bool ret = false;
  for (auto & summand : *summands) {
    if (*(summand.first) == monexpr) {
      ret = true;
      break;
    }
  }

  return ret;
}

void Expression::RedefineScalars() {
  if (summands->size() == 0) {
    return;
  }

  std::map<size_t, size_t> coefficient_map = GetCoefficientMap ();
  std::vector<std::vector<Rational>> prefactor_matrix = GetPrefactorMatrix ();

  MatrixXq mq(prefactor_matrix.size(), prefactor_matrix.front().size());

  for (size_t row_counter = 0; row_counter < prefactor_matrix.size(); ++row_counter) {
    for (size_t column_counter = 0; column_counter < prefactor_matrix.front().size(); ++column_counter) {
      Fraction frac = prefactor_matrix[row_counter][column_counter].get_fraction();
      mq(row_counter, column_counter) = mpq_class(frac.first, frac.second);
    }
  }

  Eigen::FullPivLU<MatrixXq> lu_decompq(mq);

  MatrixXq kq = lu_decompq.kernel();

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
  std::for_each(coeff_removed.begin(), coeff_removed.end(), [&ret, &coefficient_rmap, this] (auto a) { EliminateVariable(coefficient_rmap.at(a)); });

  CanonicalisePrefactors();

  std::map<size_t, size_t> substitution_map;

  std::for_each(coefficient_map.cbegin(), coefficient_map.cend(),
    [&substitution_map,&coeff_removed,n=1] (auto const & a) mutable {
      if (coeff_removed.find(a.first) == coeff_removed.end()) {
        substitution_map[a.first] = n++;
      }
    });
  
  SubstituteVariables(substitution_map);

/*
  auto summands_new = std::make_unique<Sum>();
  summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>(Scalar(1))));
  std::swap(summands->front().first, summands_new->back().first);
  
  size_t variable_counter = 0;

  auto it_last = summands->begin();

  for (auto it = summands->begin() + 1; it != summands->end(); ++it) {
    Rational coefficient(1,1);
    if (*(it->second) == *((it - 1)->second)) {
      coefficient = coefficient.DivideOther((it_last)->second->Ratio(*(it->second)));
    } else {
      ++variable_counter;
      it_last = it;
    }
    summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>(Scalar(coefficient, variable_counter + 1))));
    std::swap(it->first, summands_new->back().first);
  }

  std::swap(summands, summands_new);
*/
}

void Expression::RenameDummies() {
  std::for_each(summands->begin(), summands->end(),
    [](auto & a) {
      a.first->RenameDummies();
    });
}

void Expression::MultiplyCoefficient (Rational const & q) {
  std::for_each(summands->begin(), summands->end(),
    [q] (auto & a) {
      a.second->MultiplyCoefficient(q);
    });
}

ScalarSum Expression::EvaluateIndices(Indices const & indices, std::vector<size_t> const & numbers) const {
  std::map<Index, size_t> evaluation_map;

  for (size_t counter = 0; counter < indices.size(); ++counter) {
    evaluation_map[indices.at(counter)] = numbers.at(counter);
  }
  ScalarSum ret;
  std::for_each(summands->begin(), summands->end(),
    [&ret, &evaluation_map](auto & a) {
      ScalarSum new_summand(*(a.second));
      new_summand.MultiplyCoefficient(a.first->EvaluateIndices(evaluation_map));
      ret.MergeWithOther(new_summand);
    });

  ret.Sort();
  ret.Collect();

  return ret;
}

void Expression::AddOther(Expression const & other) {
  auto summands_new = std::make_unique<Sum>();

  summands_new->reserve(this->summands->size() + other.summands->size());
  std::for_each(summands->cbegin(), summands->cend(),
    [&summands_new] (auto const & a) {
      summands_new->emplace_back(std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second)));
    });
  std::for_each(other.summands->cbegin(), other.summands->cend(),
    [&summands_new] (auto const & a) {
      summands_new->emplace_back(std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second)));
    });

  assert(summands_new->size() == this->summands->size() + other.summands->size());

  std::swap(summands, summands_new);
}

void Expression::MultiplyOther(Expression const & other) {
  auto summands_new = std::make_unique<Sum>();

  if (!(IsZero() || other.IsZero())) {

  std::for_each(summands->cbegin(), summands->cend(),
    [&summands_new, &other] (auto const & a) {
      std::for_each(other.summands->cbegin(), other.summands->cend(),
        [&summands_new, &a] (auto const & b) {
          summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(a.first->MultiplyOther(*(b.first))), std::make_unique<ScalarSum>(a.second->MultiplyOther(*(b.second)))));
        });
      });

  }

  assert (summands_new->size() == this->summands->size() * other.summands->size());

  std::swap(summands, summands_new);
}

bool Expression::IsZero() const { return summands->empty(); }

void Expression::SaveToFile(std::string const & filename) const {
  std::ofstream file;
  file.open(filename);
  boost::archive::text_oarchive oa {file};
  oa << *this;
}

void Expression::LoadFromFile(std::string const & filename) {
  std::ifstream file;
  file.open(filename);
  boost::archive::text_iarchive ia {file};
  ia >> *this;
}

std::string const Expression::GetLatexString(std::string base_name, bool upper) const {
  std::stringstream ss;

  bool first = true;

  if (summands->empty()) {
    return "0";
  }

  for (auto const & summand : *summands) {
    if (!first) {
      ss << std::endl;
    }
    ss << summand.second->ToString(base_name, !first) << " ";
    first = false;

    ss << summand.first->GetLatexString(upper);
  }

  return ss.str();
}

bool operator< (Summand const & lop, Summand const & rop) {
  if (*lop.first < *rop.first) {
    return true;
  } else if (*rop.first < *lop.first) {
    return false;
  } else if (*lop.second < *rop.second) {
    return true;
  } else {
    return false;
  }
}

bool CompareSummandsReversed (Summand const & lop, Summand const & rop) {
  if (MonomialExpression::CompareReversed(*lop.first, *rop.first)) {
    return true;
  } else if (MonomialExpression::CompareReversed(*rop.first, *lop.first)) {
    return false;
  } else if (*lop.second < *rop.second) {
    return true;
  } else {
    return false;
  }
}
