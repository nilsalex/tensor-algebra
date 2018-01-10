#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "Indices.h"
#include "Rational.h"
#include "Tensor.h"

enum EliminateReturn {ERROR, NO_ACTION, ETA_ETA_TO_DELTA, ETA_ETA_TO_TRACE, DELTA_OK, DELTA_TO_TRACE, EPSILON_TO_ZERO};

typedef std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>> IndexMappingEntry;
typedef std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> IndexMapping;

bool operator== (IndexMappingEntry const & A, IndexMappingEntry const & B);
bool operator< (IndexMappingEntry const & A, IndexMappingEntry const & B);

class MonomialExpression {
 private:
  std::unique_ptr<IndexMapping> index_mapping;

 public:
  MonomialExpression();
  MonomialExpression(Tensor const & tensor_set, Indices const & indices_set);
  MonomialExpression(MonomialExpression const & other);

  bool IsZero() const;

  bool ApplySymmetries();
  void Sort();

  void AddFactorRight(MonomialExpression const & other);

  void ExchangeIndices(Indices const & indices1, Indices const & indices2);
  EliminateReturn EliminateEtaEta();
  EliminateReturn EliminateEpsilon();
  EliminateReturn EliminateDelta();

  Rational EvaluateIndices(std::map<Index, size_t> const & evaluation_map) const;

  MonomialExpression MultiplyOther(MonomialExpression const & other) const;

  bool operator== (MonomialExpression const & other) const;
  bool operator!= (MonomialExpression const & other) const;
  bool operator< (MonomialExpression const & other) const;

  std::string GetLatexString(bool upper = true) const;

  ~MonomialExpression() = default;
};
