#pragma once

#include <string>
#include <vector>

#include "NextCombination.h"

constexpr size_t LARGE_ENOUGH_N = 1024;

class IndexPair {
 private:
  size_t a;
  size_t b;
 
 public:
  IndexPair(size_t a, size_t b);

  size_t first() const;
  size_t second() const;

  void Sort();

  bool operator== (IndexPair const & other) const;
  bool operator< (IndexPair const & other) const;
  bool operator> (IndexPair const & other) const;

  bool Next(size_t N);
};

namespace std {
  template <>
  struct hash<IndexPair>
  {
    size_t operator()(IndexPair const & pair) const noexcept {
      return pair.first() + LARGE_ENOUGH_N * pair.second();
    }
  };
}

class MyPermutation {
 private:
  std::vector<IndexPair> perm;
  size_t N;
  size_t num = (N * (N - 1)) / 2;
  size_t n_half = N / 2;
 
 public:
  MyPermutation(size_t N);

  MyPermutation(MyPermutation const & other) = delete;
  MyPermutation(MyPermutation && other) = delete;

  std::vector<size_t> GetPermutation() const;

  bool NextCombination();

  bool NextIndexPermutation();

  bool IsIndexPermutation() const;

  std::string ToString() const;

  ~MyPermutation() = default;
};
