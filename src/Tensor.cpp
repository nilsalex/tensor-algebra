#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Tensor.h"

template <typename Archive>
void Tensor::serialize(Archive & ar, unsigned int const version) {
  if (version > 0) {
  } else {
  }
  ar & symmetric;
  ar & antisymmetric;
  ar & rank;
  ar & name;
}

template void Tensor::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int const);
template void Tensor::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, unsigned int const);

Tensor::Tensor () : rank(0), name("") { }

Tensor::Tensor (Tensor const & other) : symmetric(other.symmetric), antisymmetric(other.antisymmetric), rank(other.rank), name(other.name) { }

Tensor::Tensor (int rank_set, std::string name_set) : rank(rank_set), name(name_set) { }

std::string Tensor::get_name () const { return name; }
size_t Tensor::get_rank() const { return rank; }

void Tensor::SetSymmetric() { symmetric = true; }
void Tensor::SetAntisymmetric() { antisymmetric = true; }

bool Tensor::IsSymmetric() const { return symmetric; }
bool Tensor::IsAntisymmetric() const { return antisymmetric; }

std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> Tensor::GetIndexMapping(Indices const & indices) const {
  std::vector<std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>> ret;
  ret.push_back(std::make_pair(std::make_unique<Indices>(indices), std::make_unique<Tensor>(*this)));

  return ret;
}

bool Tensor::operator== (Tensor const & other) const {
  return (this->rank == other.rank) && (this->name == other.name);
}

bool Tensor::operator!= (Tensor const & other) const { return !(*this == other); }

bool Tensor::operator< (Tensor const & other) const {
  if (this->name < other.name) {
    return true;
  } else if (this->name > other.name) {
    return false;
  }

  return this->rank < other.rank;
}

