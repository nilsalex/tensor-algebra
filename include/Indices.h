#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/serialization/vector.hpp>

const char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

enum class Index {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z};

class Indices {
 private:
  std::vector<Index> indices;

  friend class boost::serialization::access;

  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 public:
  static bool SortIndexVector(std::vector<Index> & vec);

  Indices() = default;
  Indices(std::initializer_list<Index> list);
  Indices(std::initializer_list<char> list);

  Indices(std::vector<Index> vec);
  Indices(std::set<Index> set);

  Indices(Indices const & other);
  Indices(Indices const & other, std::map<Index, Index> const & exchange_map);

  std::vector<Index> get_vector() const;

  void SortAndMakeUnique();
  std::string const ToString() const;

  size_t size() const;
  Index at(size_t pos) const;

  bool Sort();

  Indices Permutation(std::vector<size_t> const & permutation) const;
  Indices SubIndices(size_t start, size_t len) const;

  void Replace(Index const i_old, Index const i_new);

  Indices Overlap(Indices const & other) const;

  bool operator== (Indices const & other) const;
  bool operator!= (Indices const & other) const;
  bool operator< (Indices const & other) const;

  ~Indices() = default;
};

