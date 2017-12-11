#include "TensorMonomial.h"

std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> TensorMonomial::GetIndexMapping (Indices const & indices) const {
  std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> ret;

  size_t index_counter = 0;

  for (auto & tensor : factors) {
    ret.push_back(std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>());
    auto sub_indices = std::make_unique<Indices>(indices.SubIndices(index_counter, tensor->get_rank()));
    auto tensor_copy = std::make_unique<Tensor>(*tensor);
    std::swap(ret.back().first, sub_indices);
    std::swap(ret.back().second, tensor_copy);
    index_counter += tensor->get_rank();
  }

  return ret;
}

void TensorMonomial::AddFactorRight (Tensor const & factor) {
  factors.push_back(std::make_unique<Tensor>(factor));
  rank += factor.get_rank();
}

