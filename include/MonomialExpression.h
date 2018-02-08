#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "Indices.h"
#include "Rational.h"
#include "Tensor.h"

enum Status {ERROR, NO_ACTION, ETA_ETA_TO_DELTA, ETA_ETA_TO_TRACE, ETA_CONTRACTION, ETA_PARTIAL_CONTRACTION, DELTA_OK, DELTA_TO_TRACE, EPSILON_TO_ZERO, EPSILON_EPSILONI, EPSILONI, SYM_EVEN, SYM_ODD, ZERO_ZERO, ZERO_POSITIVE, ZERO_NEGATIVE, GAMMA_GAMMA_TO_DELTA, GAMMA_GAMMA_TO_TRACE, TRACE_TO_ZERO};

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

  std::pair<std::vector<Index>, std::vector<Index>> GetFreeAndDummyIndices() const;

 public:
  MonomialExpression();
  MonomialExpression(Tensor const & tensor_set, Indices const & indices_set);
  MonomialExpression(MonomialExpression const & other);

  bool IsZero() const;

  bool ApplySymmetries();
  void Sort();

  void AddFactorRight(MonomialExpression const & other);

  void ExchangeIndices(Indices const & indices1, Indices const & indices2);

  std::vector<Index> GetFreeIndices() const;
  std::vector<Index> GetNonFreeIndices() const;
  std::vector<Index> GetDummyIndices() const;
  std::vector<Index> GetUnusedIndices() const;

  Status ApplySymmetriesToContractions();

  Status EliminateGammaGamma();
  Status EliminateEtaEta();
  Status EliminateMetricMetric(unsigned int const dimension);
  Status EliminateEtaPartial();
  Status EliminateEtaRankOne();
  Status EliminateEpsilon(unsigned int const dimension);
  Status EliminateEpsilonI();
  std::pair<Indices, Indices> EliminateEpsilonEpsilonI(unsigned int const dimension);
  Status EliminateGamma();
  Status EliminateDelta(std::string const & delta_name = "delta");
  Status EliminateTracefree();

  Status ThreePlusOne(std::vector<Index> indices_to_zero);
  
  void RenameDummies();

  Rational EvaluateIndices(std::map<Index, size_t> const & evaluation_map) const;

  MonomialExpression MultiplyOther(MonomialExpression const & other) const;

  bool operator== (MonomialExpression const & other) const;
  bool operator!= (MonomialExpression const & other) const;
  bool operator< (MonomialExpression const & other) const;

  std::string GetLatexString(bool upper = true) const;

  static bool CompareReversed (MonomialExpression const & lop, MonomialExpression const & rop);

  ~MonomialExpression() = default;
};
