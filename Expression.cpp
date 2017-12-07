#include <iostream>
#include <sstream>

#include "Expression.h"
#include "MonomialExpression.h"
#include "Scalar.h"
#include "ScalarSum.h"

void Expression::AddSummand (MonomialExpression const & monomial_expression, Scalar const & scalar) {
  summands->push_back(std::make_pair(std::make_unique<MonomialExpression>(monomial_expression), std::make_unique<ScalarSum>(scalar)));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression) {
  AddSummand(monomial_expression, Scalar());
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, Rational const & rational) {
  AddSummand(monomial_expression, Scalar(rational));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, std::string variable) {
  AddSummand(monomial_expression, Scalar(variable));
}

void Expression::AddSummand (MonomialExpression const & monomial_expression, Rational const & rational, std::string variable) {
  AddSummand(monomial_expression, Scalar(rational, variable));
}

void Expression::SortMonomials() {
  for (auto & summand : *summands) {
    summand.first->Sort();
  }
}

void Expression::ApplyMonomialSymmetries() {
  for (auto & summand : *summands) {
    bool even = summand.first->ApplySymmetries();
    if (!even) {
      summand.second->Negate();
    }
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
  for (auto & summand : *summands) {
    summand.second->Sort();
    summand.second->Collect();
  }

  EliminateZeros();
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

void Expression::SortSummands() {
  bool swapped;
  do {
    swapped = false;
    for (auto it = summands->begin(); std::distance(it, summands->end()) > 1; ++it) {
      if (*(it->first) < *(std::next(it)->first)) {
        std::swap(*it, *std::next(it));
        swapped = true;
      }
    }
  } while (swapped);
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
  std::sort(summands->begin(), summands->end(), [](auto & a, auto & b) { return *(a.second) < *(b.second); });
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

void Expression::RedefineScalars(std::string const & base_name) {
  if (summands->size() == 0) {
    return;
  }

  auto summands_new = std::make_unique<Sum>();
  summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>(Scalar(base_name + "_" + std::to_string(1)))));
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
    summands_new->push_back(std::make_pair(std::make_unique<MonomialExpression>(), std::make_unique<ScalarSum>(Scalar(coefficient, base_name + "_" + std::to_string(variable_counter + 1)))));
    std::swap(it->first, summands_new->back().first);
  }

  std::swap(summands, summands_new);
}

std::pair<std::set<size_t>, ScalarSum> Expression::EvaluateIndices(Indices const & indices, std::vector<size_t> const & numbers) const {
  std::map<Index, size_t> evaluation_map;

  for (size_t counter = 0; counter < indices.size(); ++counter) {
    evaluation_map[indices.at(counter)] = numbers.at(counter);
  }
  auto ret = std::make_pair(std::set<size_t>(), ScalarSum());
  std::for_each(summands->begin(), summands->end(),
    [&ret, &evaluation_map](auto & a) {
      ScalarSum new_summand(*(a.second));
      new_summand.MultiplyCoefficient(a.first->EvaluateIndices(evaluation_map));
      ret.second.MergeWithOther(new_summand);
      ret.first.merge(a.second->CoefficientSet());
    });

  std::cout << ret.second.ToString() << std::endl;

  ret.second.Sort();
  ret.second.Collect();

  return ret;
}

std::string const Expression::GetLatexString(bool upper) const {
  std::stringstream ss;

  bool first = true;

  if (summands->empty()) {
    return "0";
  }

  for (auto const & summand : *summands) {
    ss << summand.second->ToString(!first) << " ";
    first = false;

    ss << summand.first->GetLatexString(upper) << "\n";
  }

  return ss.str();
}
