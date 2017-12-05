#pragma once

#include <map>
#include <string>
#include <vector>

const char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

enum class Index {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z};

class Indices {
 private:
  std::vector<Index> indices;

 public:
  Indices() = default;
  Indices(std::initializer_list<Index> list) : indices(list) { };
  Indices(std::initializer_list<char> list);

  Indices(Indices const & other) : indices(other.indices) { };
  Indices(Indices const & other, std::map<Index, Index> const & exchange_map);

  void SortAndMakeUnique();
  std::string const ToString() const;

  size_t const size() const { return indices.size(); };
  Index at(size_t pos) const { return indices.at(pos); };

  bool Sort();

  Indices Permutation(std::vector<size_t> const & permutation) const;
  Indices SubIndices(size_t start, size_t len) const;

  bool operator== (Indices const & other) const { return indices == other.indices; };
  bool operator< (Indices const & other) const { return indices < other.indices; };

  ~Indices() = default;
};

