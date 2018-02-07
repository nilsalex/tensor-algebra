#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <boost/serialization/string.hpp>

#include "Indices.h"

class Tensor {
 private:
  bool symmetric;
  bool antisymmetric;
  bool tracefree;
  int sort_index;

  friend class boost::serialization::access;
  template <typename Archive>
  void serialize(Archive & ar, unsigned int const version);

 protected:
  size_t rank;
  std::string name;

 public:
  Tensor ();

  Tensor (Tensor const & other);

  Tensor (int rank_set, std::string const & name_set = "", int sort_index_set = 0);

  std::string get_name () const;
  size_t get_rank() const;

  void SetSymmetric();
  void SetAntisymmetric();
  void SetTracefree();

  virtual std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> GetIndexMapping (Indices const & indices) const;
  bool IsSymmetric() const;
  bool IsAntisymmetric() const;
  bool IsTracefree() const;

  virtual bool operator< (Tensor const & other) const;
  virtual bool operator== (Tensor const & other) const;
  virtual bool operator!= (Tensor const & other) const;

  virtual ~Tensor() = default;
};

