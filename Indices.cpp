#include <algorithm>

#include "Indices.h"

Indices::Indices(Indices const & other, std::map<Index, Index> const & exchange_map) : indices(std::vector<Index>()){
  for (Index index : other.indices) {
    indices.push_back(exchange_map.at(index));
  }
}

void Indices::SortAndMakeUnique() {
  std::sort(indices.begin(), indices.end());
  indices.resize(std::distance(indices.begin(), std::unique(indices.begin(), indices.end())));
}

bool Indices::Sort() {
  unsigned int swap_counter = 0;
  bool swapped;
  do {
    swapped = false;
    for (size_t counter = 0; counter < indices.size() - 1; ++counter) {
      if (indices[counter] > indices[counter + 1]) {
        std::swap(indices[counter], indices[counter + 1]);
        swapped = true;
        swap_counter += 1;
      }
    }
  } while (swapped);
  return (swap_counter % 2 == 0) ? true : false; 
}

std::string const Indices::ToString() const {
  std::string return_string("");
  return_string += "{ ";
  for (auto index : indices) {
    return_string += alphabet[static_cast<int>(index)];
    return_string += " ";
  }
  return_string += "}";

  return return_string;
}

Indices Indices::SubIndices(size_t start, size_t len) const {
  Indices indices_ret;
  indices_ret.indices = std::vector<Index>(indices.begin() + start, indices.begin() + start + len);
  return indices_ret;
}

