#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Expression.h"
#include "MonomialExpression.h"
#include "Scalar.h"
#include "ScalarSum.h"

template <typename Archive>
void Expression::serialize(Archive & ar, unsigned int const version) {
  if (version > 0) {
  } else {
  }
  ar & summands;
}

template void Expression::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int const);
template void Expression::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, unsigned int const);

Expression::Expression() : summands(std::make_unique<std::vector<Summand>>()) { }

Expression::Expression(Expression const & other) : summands(std::make_unique<Sum>()) {
  std::for_each(other.summands->cbegin(), other.summands->cend(), [this](auto const & it) {
    summands->push_back(std::pair<std::unique_ptr<MonomialExpression>, std::unique_ptr<ScalarSum>>());
    auto monexp_new = std::make_unique<MonomialExpression>(*(it.first));
    auto scalar_new = std::make_unique<ScalarSum>(*(it.second));
    std::swap((*(--summands->end())).first, monexp_new);
    std::swap((*(--summands->end())).second, scalar_new);
  });
}

Expression & Expression::operator=(Expression const & other) {
  Expression tmp { other };
  std::swap(summands, tmp.summands);
  return *this;
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

void Expression::EliminateDelta() {
  bool repeat = true;

  while (repeat) {
    auto summands_new = std::make_unique<Sum>();
    repeat = false;
    std::transform(summands->cbegin(), summands->cend(), std::back_inserter(*summands_new),
      [&repeat](auto const & a) {
        auto summand_new = std::make_pair(std::make_unique<MonomialExpression>(*a.first), std::make_unique<ScalarSum>(*a.second));;
        Status ret = summand_new.first->EliminateDelta();
        if (ret == DELTA_OK || ret == DELTA_TO_TRACE) {
          repeat = true;
        } else if (ret == ERROR) {
          assert(false);
        }
        return summand_new;
      });
    std::swap(summands, summands_new);
  }

}

void Expression::EliminateEpsilon() {
  bool repeat = true;

  while (repeat) {
    repeat = false;
    std::for_each(summands->begin(), summands->end(),
      [&repeat](auto & a) {
        Status ret = a.first->EliminateEpsilon();
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
        if (ret == ETA_PARTIAL_TO_BOX) {
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

void Expression::EliminateVariable(size_t const variable) {
  std::for_each(summands->begin(), summands->end(), [variable](auto & a) { a.second->EliminateVariable(variable); });
}

void Expression::SortSummands() {
  std::sort(summands->begin(), summands->end());
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
    summand_copy.first->ExchangeIndices(indices1, indices2);
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
}

void Expression::RenameDummies() {
  std::for_each(summands->begin(), summands->end(),
    [](auto & a) {
      a.first->RenameDummies();
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

Expression Expression::MultiplyOther(Expression const & other) const {
  if (IsZero() || other.IsZero()) {
    return Expression();
  }

  Expression ret;

  std::for_each(summands->cbegin(), summands->cend(),
    [&ret, &other] (auto const & a) {
      std::for_each(other.summands->cbegin(), other.summands->cend(),
        [&ret, &a] (auto const & b) {
          ret.AddSummand(a.first->MultiplyOther(*(b.first)), a.second->MultiplyOther(*(b.second)));
        });
      });

  return ret;
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
