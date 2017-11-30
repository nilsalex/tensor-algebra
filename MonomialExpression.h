#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Indices.h"
#include "Tensor.h"

typedef std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>> IndexMappingEntry;
typedef std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> IndexMapping;

bool operator== (IndexMappingEntry const & A, IndexMappingEntry const & B);
bool operator< (IndexMappingEntry const & A, IndexMappingEntry const & B);

class MonomialExpression {
 private:
  std::unique_ptr<IndexMapping> index_mapping;

 public:
  MonomialExpression() : index_mapping(std::make_unique<IndexMapping>()) { };
  MonomialExpression(Tensor const & tensor_set, Indices const & indices_set);
  MonomialExpression(MonomialExpression const & other);

  bool ApplySymmetries();
  void Sort();

  void ExchangeIndices(Indices const & indices1, Indices const & indices2);

  bool operator== (MonomialExpression const & other) const { return *index_mapping == *other.index_mapping; };
  bool operator< (MonomialExpression const & other) const { return *index_mapping < *other.index_mapping; };

  std::string GetLatexString(bool upper = true) const;

  ~MonomialExpression() = default;
};
