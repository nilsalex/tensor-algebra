#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "NextCombination.h"

constexpr size_t LARGE_ENOUGH_N = 100;

class IndexPair {
 private:
  size_t a;
  size_t b;
 
 public:
  IndexPair(size_t a, size_t b) : a(a), b(b) { };

  size_t first() const { return a; } ;
  size_t second() const { return b; } ;

  void Sort() { if (a <= b) return; std::swap(a, b); return; };

  bool operator== (IndexPair const & other) const { return (a == other.a && b == other.b); };
  bool operator< (IndexPair const & other) const { return (a < other.a ? true : (a > other.a ? false : (b < other.b ? true : false))); };
  bool operator> (IndexPair const & other) const { return other < *this; };

  bool Next(size_t N) {
    if (b + 1 < N) {
      b++;
      return true;
    } else if (a + 2 < N) {
      a++;
      b = a + 1;
      return true;
    } else {
      a = 0;
      b = 1;
      return false;
    }
  };
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
  MyPermutation(size_t N) : N(N), num((N*(N-1))/2), n_half(N/2) {
    IndexPair pair(0, 1);

    do {
      perm.push_back(IndexPair(pair));
    } while (pair.Next(N));
  };

  MyPermutation(MyPermutation const & other) = delete;
  MyPermutation(MyPermutation && other) = delete;

  std::vector<size_t> GetPermutation() const {
    std::vector<size_t> ret(N);
    std::generate(ret.begin(), ret.end(), [this, n=0] () mutable { if (n++ % 2 == 0) { return perm[(n-1)/2].first(); } else { return perm[(n-1)/2].second(); } });
    return ret;
  }

  bool NextCombination() {
    return next_combination(perm.begin(), perm.begin() + n_half, perm.end());
  }

  bool NextIndexPermutation() {
    do {
      if (!(this->NextCombination())) {
        return false;
      }
    } while (!(this->IsIndexPermutation()));
    return true;
  }

  bool IsIndexPermutation() const {
    std::unordered_set<IndexPair> set;
    for (size_t pair_counter = 0; pair_counter < n_half; ++pair_counter) {
      if (std::find_if(set.cbegin(), set.cend(),
        [this,pair_counter] (auto const & other) {
          return (this->perm[pair_counter].first() == other.first() || this->perm[pair_counter].second() == other.second() || this->perm[pair_counter].first() == other.second() || this->perm[pair_counter].second() == other.first());
        }
        ) != set.end()) {
        return false;
      }
      set.insert(perm[pair_counter]);
    }
    return true;
  };

  std::string ToString() const {
    std::stringstream ss;
    for (size_t counter = 0; counter < n_half; ++counter) {
      auto const & pair = perm.at(counter);
      ss << std::to_string(pair.first());
      ss << " ";
      ss << std::to_string(pair.second());
      ss << " ";
    }
    return ss.str();
  }

  ~MyPermutation() = default;
};
