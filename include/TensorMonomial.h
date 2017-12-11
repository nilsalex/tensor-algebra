#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "Indices.h"
#include "Tensor.h"

class TensorMonomial : public Tensor {
 private:
  std::vector<std::unique_ptr<Tensor>> factors;

 public:
  TensorMonomial () : Tensor(0) { };

  void AddFactorRight (Tensor const & factor);

  virtual std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> GetIndexMapping (Indices const & indices) const;

//  virtual bool operator< (Tensor const & other) const;
//  virtual bool operator= (Tensor const & other) const;

  virtual ~TensorMonomial() = default;
};

