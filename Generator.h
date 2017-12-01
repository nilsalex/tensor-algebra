#pragma once

#include <memory>

#include "Expression.h"
#include "Indices.h"
#include "Tensor.h"

class Generator {
 private:
  size_t rank;
  Indices indices;
  Tensor eta;
  Tensor epsilon;
  std::vector<std::pair<Indices, bool>> symmetries;

  Expression expression;

 public:
  Generator() = delete;
  Generator(Indices const & indices_set, std::initializer_list<std::pair<Indices, bool>> const & symmetries_set);

  Expression Generate();

  ~Generator() = default;
};
