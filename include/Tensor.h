#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Indices.h"

class Tensor {
 private:
  bool symmetric = false;
  bool antisymmetric = false;

 protected:
  size_t rank;
  std::string name;

 public:
  Tensor () : rank(0), name("") { };

  Tensor (Tensor const & other) : rank(other.rank), name(other.name), symmetric(other.symmetric), antisymmetric(other.antisymmetric) { };

  Tensor (int rank_set, std::string name_set = "") : rank(rank_set), name(name_set) { };

  std::string const get_name () const { return name; };
  size_t const get_rank() const { return rank; }; 

  void SetSymmetric() { symmetric = true; };
  void SetAntisymmetric() { antisymmetric = true; };

  virtual std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> GetIndexMapping (Indices const & indices) const;
  bool IsSymmetric() const { return symmetric; };
  bool IsAntisymmetric() const { return antisymmetric; };

  virtual bool operator< (Tensor const & other) const;
  virtual bool operator== (Tensor const & other) const;
  virtual bool operator!= (Tensor const & other) const { return !(*this == other); };
//  virtual bool operator> (Tensor const & other) const { return other < *this; };
//  virtual bool operator<= (Tensor const & other) const { return !(other < *this); };
//  virtual bool operator>= (Tensor const & other) const { return !(*this < other); };
//
  virtual ~Tensor() = default;
};

