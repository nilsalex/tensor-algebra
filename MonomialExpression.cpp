#include "MonomialExpression.h"

#include <algorithm>
#include <cassert>
#include <sstream>

bool operator== (IndexMappingEntry const & A, IndexMappingEntry const & B) {
  return (*A.first == *B.first) && (*A.second == *B.second);;
}

bool operator< (IndexMappingEntry const & A, IndexMappingEntry const & B) {
  if (*(A.second) < *(B.second)) {
    return true;
  } else if (*(B.second) < *(A.second)) {
    return false;
  }

  return *(A.first) < *(B.first);
}

MonomialExpression::MonomialExpression(Tensor const & tensor_set, Indices const & indices_set) : index_mapping(std::make_unique<IndexMapping>()) { 
  auto mapping_set = std::make_unique<IndexMapping>(tensor_set.GetIndexMapping(indices_set));
  std::swap(index_mapping, mapping_set);
};

MonomialExpression::MonomialExpression(MonomialExpression const & other) : index_mapping(std::make_unique<IndexMapping>()) { 
  auto vec = std::make_unique<IndexMapping>();

  for (auto & pair : *other.index_mapping) {
    vec->push_back(std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>());
    auto indices_set = std::make_unique<Indices>(*pair.first);
    auto tensor_set = std::make_unique<Tensor>(*pair.second);
    std::swap((--vec->end())->first, indices_set);
    std::swap((--vec->end())->second, tensor_set);
  }

  std::swap(index_mapping, vec);
};

void MonomialExpression::Sort() {
  std::sort(index_mapping->begin(), index_mapping->end());
}

bool MonomialExpression::ApplySymmetries() {
  bool even = true;

  for (auto & pair : *index_mapping) {
    if (pair.second->IsSymmetric()) {
      pair.first->Sort(); 
    } else if (pair.second->IsAntisymmetric()) {
      if (!pair.first->Sort()) {
        even = !even;
      }
    }
  }

  return even;
}

Rational MonomialExpression::EvaluateIndices(std::map<Index, size_t> const & evaluation_map) const {
  int ret = 1;
  for (auto & entry : *index_mapping) {
    if (entry.second->get_name() == "eta") {
      if (evaluation_map.at(entry.first->at(0)) != evaluation_map.at(entry.first->at(1))) {
        return Rational(0,1);
      } else if (evaluation_map.at(entry.first->at(0)) > 0) {
        ret = -ret;
      }
    } else if (entry.second->get_name() == "epsilon") {
      std::vector<size_t> epsilon_eval (4);
      epsilon_eval[0] = evaluation_map.at(entry.first->at(0));
      epsilon_eval[1] = evaluation_map.at(entry.first->at(1));
      epsilon_eval[2] = evaluation_map.at(entry.first->at(2));
      epsilon_eval[3] = evaluation_map.at(entry.first->at(3));

      if(std::distance(epsilon_eval.begin(), std::unique(epsilon_eval.begin(), epsilon_eval.end())) < 4) {
        return Rational(0, 1);
      }

      bool anti = true;

      while (std::next_permutation(epsilon_eval.begin(), epsilon_eval.end())) {
        anti = !anti;
      }

      if (anti) {
        ret = -ret;
      }

    } else {
      assert(false);
    }
  }

  return Rational(ret, 1);
}

void MonomialExpression::ExchangeIndices(Indices const & indices1, Indices const & indices2) {
  std::map<Index, Index> exchange_map;
  for (size_t index_counter = 0; index_counter < indices1.size(); ++index_counter) {
    exchange_map[indices1.at(index_counter)] = indices2.at(index_counter);
  }

  auto index_mapping_new = std::make_unique<IndexMapping>();
  for (auto & entry : *index_mapping) {
    auto indices_new = std::make_unique<Indices>(*(entry.first), exchange_map);
    index_mapping_new->push_back(std::make_pair(std::make_unique<Indices>(), std::make_unique<Tensor>()));
    std::swap((--(index_mapping_new->end()))->first, indices_new);
    std::swap((--(index_mapping_new->end()))->second, entry.second);
  }

  std::swap(index_mapping, index_mapping_new);
}

std::string MonomialExpression::GetLatexString(bool upper) const {
  std::stringstream ss;

  for (auto & pair : *index_mapping) {
    ss << pair.second->get_name() << (upper ? "^" : "_") << pair.first->ToString() << " ";
  }

  return ss.str();
}

