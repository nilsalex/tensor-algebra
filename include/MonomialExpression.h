#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "Indices.h"
#include "Rational.h"
#include "Tensor.h"

enum Status {ERROR, NO_ACTION, ETA_ETA_TO_DELTA, ETA_ETA_TO_TRACE, ETA_PARTIAL_TO_BOX, DELTA_OK, DELTA_TO_TRACE, EPSILON_TO_ZERO, SYM_EVEN, SYM_ODD};

typedef std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>> IndexMappingEntry;
typedef std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> IndexMapping;

bool operator== (IndexMappingEntry const & A, IndexMappingEntry const & B);
bool operator< (IndexMappingEntry const & A, IndexMappingEntry const & B);

class MonomialExpression {
 private:
  std::unique_ptr<IndexMapping> index_mapping;

  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  MonomialExpression();
  MonomialExpression(Tensor const & tensor_set, Indices const & indices_set);
  MonomialExpression(MonomialExpression const & other);

  bool IsZero() const;

  bool ApplySymmetries();
  void Sort();

  void AddFactorRight(MonomialExpression const & other);

  void ExchangeIndices(Indices const & indices1, Indices const & indices2);

  Status ApplySymmetriesToContractions();

  Status EliminateEtaEta();
  Status EliminateEtaPartial();
  Status EliminateEpsilon();
  Status EliminateDelta();
  
  void RenameDummies();

  Rational EvaluateIndices(std::map<Index, size_t> const & evaluation_map) const;

  MonomialExpression MultiplyOther(MonomialExpression const & other) const;

  bool operator== (MonomialExpression const & other) const;
  bool operator!= (MonomialExpression const & other) const;
  bool operator< (MonomialExpression const & other) const;

  std::string GetLatexString(bool upper = true) const;

  ~MonomialExpression() = default;
};
