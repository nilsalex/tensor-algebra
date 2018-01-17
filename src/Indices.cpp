#include <algorithm>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Indices.h"

template <typename Archive>
void Indices::serialize(Archive & ar, unsigned int const version) {
  if (version > 0) {
  } else {
  }
  ar & indices;
}

template void Indices::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int const);
template void Indices::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, unsigned int const);

bool Indices::SortIndexVector(std::vector<Index> & vec) {
  Indices ind (vec);
  bool ret = ind.Sort();
  vec = ind.get_vector();
  return ret;
}

Indices::Indices(std::initializer_list<Index> list) : indices(list) { }

Indices::Indices(std::initializer_list<char> list) {
  indices.resize(list.size());
  std::transform(list.begin(), list.end(), indices.begin(),
    [](auto c) {
      return static_cast<Index>(c - 97);
    });
}

Indices::Indices(std::vector<Index> vec) : indices(vec) { }

Indices::Indices(std::set<Index> set)    : indices(set.begin(), set.end()) { }

Indices::Indices(Indices const & other) : indices(other.indices) { }

Indices::Indices(Indices const & other, std::map<Index, Index> const & exchange_map) : indices(std::vector<Index>()){
  for (Index index : other.indices) {
    indices.push_back(exchange_map.at(index));
  }
}

std::vector<Index> Indices::get_vector() const {
  return indices;
}

void Indices::SortAndMakeUnique() {
  std::sort(indices.begin(), indices.end());
  indices.resize(std::distance(indices.begin(), std::unique(indices.begin(), indices.end())));
}

bool Indices::Sort() {
  if (indices.empty()) {
    return true;
  }
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

size_t Indices::size() const { return indices.size(); }
Index Indices::at(size_t pos) const { return indices.at(pos); }

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

Indices Indices::Permutation(std::vector<size_t> const & permutation) const {
  if (indices.empty()) {
    return (Indices());
  }

  Indices indices_new = *this;

  for (size_t counter = 0; counter < indices.size(); ++counter) {
    indices_new.indices[counter] = indices[permutation[counter]];
  }

  return indices_new;
}

Indices Indices::SubIndices(size_t start, size_t len) const {
  Indices indices_ret;
  indices_ret.indices = std::vector<Index>(indices.begin() + start, indices.begin() + start + len);
  return indices_ret;
}

Indices Indices::Overlap(Indices const & other) const {
  Indices ret;

  std::for_each(this->indices.cbegin(), this->indices.cend(),
    [&ret, &other] (auto const & a) mutable {
      if (std::find(other.indices.cbegin(), other.indices.cend(), a) != other.indices.end()) {
        ret.indices.push_back(a);
      }
    });

  ret.SortAndMakeUnique();

  return ret;
}

void Indices::Replace(Index const i_old, Index const i_new) {
  std::replace(indices.begin(), indices.end(), i_old, i_new);
}

bool Indices::operator== (Indices const & other) const { return indices == other.indices; }
bool Indices::operator!= (Indices const & other) const { return !(*this == other); }
bool Indices::operator< (Indices const & other) const { return indices < other.indices; }
