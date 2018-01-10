#include "Expression.h"

Expression Expression::ApplyGaugeSymmetry(Expression const & delta) const {
  Expression ret = this->MultiplyOther(delta);

  ret.SortMonomials();
  ret.EliminateEtaEta();
  ret.SortMonomials();
  ret.EliminateDelta();
  ret.SortMonomials();
  ret.EliminateEpsilon();

  return ret;
}
