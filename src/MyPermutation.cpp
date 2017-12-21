#include "MyPermutation.h"

#include <sstream>
#include <unordered_set>

IndexPair::IndexPair(size_t a, size_t b) : a(a), b(b) { }

size_t IndexPair::first() const { return a; }
size_t IndexPair::second() const { return b; }

void IndexPair::Sort() { if (a <= b) return; std::swap(a, b); return; }

bool IndexPair::operator== (IndexPair const & other) const { return (a == other.a && b == other.b); }
bool IndexPair::operator< (IndexPair const & other) const { return (a < other.a ? true : (a > other.a ? false : (b < other.b ? true : false))); }
bool IndexPair::operator> (IndexPair const & other) const { return other < *this; }

bool IndexPair::Next(size_t N) {
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
}

MyPermutation::MyPermutation(size_t N) : N(N), num((N*(N-1))/2), n_half(N/2) {
  IndexPair pair(0, 1);

  do {
    perm.push_back(IndexPair(pair));
  } while (pair.Next(N));

  this->NextIndexPermutation();
}

std::vector<size_t> MyPermutation::GetPermutation() const {
  std::vector<size_t> ret(N);
  std::generate(ret.begin(), ret.end(), [this, n=0] () mutable { if (n++ % 2 == 0) { return perm[(n-1)/2].first(); } else { return perm[(n-1)/2].second(); } });
  return ret;
}

bool MyPermutation::NextCombination() {
  return next_combination(perm.begin(), perm.begin() + n_half, perm.end());
}

bool MyPermutation::NextIndexPermutation() {
  do {
    if (!(this->NextCombination())) {
      return false;
    }
  } while (!(this->IsIndexPermutation()));
  return true;
}

bool MyPermutation::IsIndexPermutation() const {
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
}

std::string MyPermutation::ToString() const {
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
