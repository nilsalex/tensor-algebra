#include "Tensor.h"

std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> Tensor::GetIndexMapping(Indices const & indices) const {
  std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> ret;
  ret.push_back(std::make_pair(std::make_unique<Indices>(indices), std::make_unique<Tensor>(*this)));

  return ret;
}

bool Tensor::operator== (Tensor const & other) const {
  return (this->rank == other.rank) && (this->name == other.name);
}

bool Tensor::operator< (Tensor const & other) const {
  if (this->name < other.name) {
    return true;
  } else if (this->name > other.name) {
    return false;
  }

  return this->rank < other.rank;
}

