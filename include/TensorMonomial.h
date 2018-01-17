#pragma once

#include <memory>
#include <utility>
#include <vector>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>

#include "Indices.h"
#include "Tensor.h"

class TensorMonomial : public Tensor {
 private:
  std::vector<std::unique_ptr<Tensor>> factors;

  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  TensorMonomial ();

  void AddFactorRight (Tensor const & factor);

  virtual std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> GetIndexMapping (Indices const & indices) const;

  virtual ~TensorMonomial() = default;
};

