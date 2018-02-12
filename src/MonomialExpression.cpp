#include "MonomialExpression.h"

#include <algorithm>
#include <cassert>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

std::map<size_t, int> const epsilon_eval {
{ 0 * 64 + 0 * 16 + 0 * 4 + 0 , 0},
{ 0 * 64 + 0 * 16 + 0 * 4 + 1 , 0},
{ 0 * 64 + 0 * 16 + 0 * 4 + 2 , 0},
{ 0 * 64 + 0 * 16 + 0 * 4 + 3 , 0},
{ 0 * 64 + 0 * 16 + 1 * 4 + 0 , 0},
{ 0 * 64 + 0 * 16 + 1 * 4 + 1 , 0},
{ 0 * 64 + 0 * 16 + 1 * 4 + 2 , 0},
{ 0 * 64 + 0 * 16 + 1 * 4 + 3 , 0},
{ 0 * 64 + 0 * 16 + 2 * 4 + 0 , 0},
{ 0 * 64 + 0 * 16 + 2 * 4 + 1 , 0},
{ 0 * 64 + 0 * 16 + 2 * 4 + 2 , 0},
{ 0 * 64 + 0 * 16 + 2 * 4 + 3 , 0},
{ 0 * 64 + 0 * 16 + 3 * 4 + 0 , 0},
{ 0 * 64 + 0 * 16 + 3 * 4 + 1 , 0},
{ 0 * 64 + 0 * 16 + 3 * 4 + 2 , 0},
{ 0 * 64 + 0 * 16 + 3 * 4 + 3 , 0},
{ 0 * 64 + 1 * 16 + 0 * 4 + 0 , 0},
{ 0 * 64 + 1 * 16 + 0 * 4 + 1 , 0},
{ 0 * 64 + 1 * 16 + 0 * 4 + 2 , 0},
{ 0 * 64 + 1 * 16 + 0 * 4 + 3 , 0},
{ 0 * 64 + 1 * 16 + 1 * 4 + 0 , 0},
{ 0 * 64 + 1 * 16 + 1 * 4 + 1 , 0},
{ 0 * 64 + 1 * 16 + 1 * 4 + 2 , 0},
{ 0 * 64 + 1 * 16 + 1 * 4 + 3 , 0},
{ 0 * 64 + 1 * 16 + 2 * 4 + 0 , 0},
{ 0 * 64 + 1 * 16 + 2 * 4 + 1 , 0},
{ 0 * 64 + 1 * 16 + 2 * 4 + 2 , 0},
{ 0 * 64 + 1 * 16 + 2 * 4 + 3 , 1},
{ 0 * 64 + 1 * 16 + 3 * 4 + 0 , 0},
{ 0 * 64 + 1 * 16 + 3 * 4 + 1 , 0},
{ 0 * 64 + 1 * 16 + 3 * 4 + 2 , -1},
{ 0 * 64 + 1 * 16 + 3 * 4 + 3 , 0},
{ 0 * 64 + 2 * 16 + 0 * 4 + 0 , 0},
{ 0 * 64 + 2 * 16 + 0 * 4 + 1 , 0},
{ 0 * 64 + 2 * 16 + 0 * 4 + 2 , 0},
{ 0 * 64 + 2 * 16 + 0 * 4 + 3 , 0},
{ 0 * 64 + 2 * 16 + 1 * 4 + 0 , 0},
{ 0 * 64 + 2 * 16 + 1 * 4 + 1 , 0},
{ 0 * 64 + 2 * 16 + 1 * 4 + 2 , 0},
{ 0 * 64 + 2 * 16 + 1 * 4 + 3 , -1},
{ 0 * 64 + 2 * 16 + 2 * 4 + 0 , 0},
{ 0 * 64 + 2 * 16 + 2 * 4 + 1 , 0},
{ 0 * 64 + 2 * 16 + 2 * 4 + 2 , 0},
{ 0 * 64 + 2 * 16 + 2 * 4 + 3 , 0},
{ 0 * 64 + 2 * 16 + 3 * 4 + 0 , 0},
{ 0 * 64 + 2 * 16 + 3 * 4 + 1 , 1},
{ 0 * 64 + 2 * 16 + 3 * 4 + 2 , 0},
{ 0 * 64 + 2 * 16 + 3 * 4 + 3 , 0},
{ 0 * 64 + 3 * 16 + 0 * 4 + 0 , 0},
{ 0 * 64 + 3 * 16 + 0 * 4 + 1 , 0},
{ 0 * 64 + 3 * 16 + 0 * 4 + 2 , 0},
{ 0 * 64 + 3 * 16 + 0 * 4 + 3 , 0},
{ 0 * 64 + 3 * 16 + 1 * 4 + 0 , 0},
{ 0 * 64 + 3 * 16 + 1 * 4 + 1 , 0},
{ 0 * 64 + 3 * 16 + 1 * 4 + 2 , 1},
{ 0 * 64 + 3 * 16 + 1 * 4 + 3 , 0},
{ 0 * 64 + 3 * 16 + 2 * 4 + 0 , 0},
{ 0 * 64 + 3 * 16 + 2 * 4 + 1 , -1},
{ 0 * 64 + 3 * 16 + 2 * 4 + 2 , 0},
{ 0 * 64 + 3 * 16 + 2 * 4 + 3 , 0},
{ 0 * 64 + 3 * 16 + 3 * 4 + 0 , 0},
{ 0 * 64 + 3 * 16 + 3 * 4 + 1 , 0},
{ 0 * 64 + 3 * 16 + 3 * 4 + 2 , 0},
{ 0 * 64 + 3 * 16 + 3 * 4 + 3 , 0},

{ 1 * 64 + 0 * 16 + 0 * 4 + 0 , 0},
{ 1 * 64 + 0 * 16 + 0 * 4 + 1 , 0},
{ 1 * 64 + 0 * 16 + 0 * 4 + 2 , 0},
{ 1 * 64 + 0 * 16 + 0 * 4 + 3 , 0},
{ 1 * 64 + 0 * 16 + 1 * 4 + 0 , 0},
{ 1 * 64 + 0 * 16 + 1 * 4 + 1 , 0},
{ 1 * 64 + 0 * 16 + 1 * 4 + 2 , 0},
{ 1 * 64 + 0 * 16 + 1 * 4 + 3 , 0},
{ 1 * 64 + 0 * 16 + 2 * 4 + 0 , 0},
{ 1 * 64 + 0 * 16 + 2 * 4 + 1 , 0},
{ 1 * 64 + 0 * 16 + 2 * 4 + 2 , 0},
{ 1 * 64 + 0 * 16 + 2 * 4 + 3 , -1},
{ 1 * 64 + 0 * 16 + 3 * 4 + 0 , 0},
{ 1 * 64 + 0 * 16 + 3 * 4 + 1 , 0},
{ 1 * 64 + 0 * 16 + 3 * 4 + 2 , 1},
{ 1 * 64 + 0 * 16 + 3 * 4 + 3 , 0},
{ 1 * 64 + 1 * 16 + 0 * 4 + 0 , 0},
{ 1 * 64 + 1 * 16 + 0 * 4 + 1 , 0},
{ 1 * 64 + 1 * 16 + 0 * 4 + 2 , 0},
{ 1 * 64 + 1 * 16 + 0 * 4 + 3 , 0},
{ 1 * 64 + 1 * 16 + 1 * 4 + 0 , 0},
{ 1 * 64 + 1 * 16 + 1 * 4 + 1 , 0},
{ 1 * 64 + 1 * 16 + 1 * 4 + 2 , 0},
{ 1 * 64 + 1 * 16 + 1 * 4 + 3 , 0},
{ 1 * 64 + 1 * 16 + 2 * 4 + 0 , 0},
{ 1 * 64 + 1 * 16 + 2 * 4 + 1 , 0},
{ 1 * 64 + 1 * 16 + 2 * 4 + 2 , 0},
{ 1 * 64 + 1 * 16 + 2 * 4 + 3 , 0},
{ 1 * 64 + 1 * 16 + 3 * 4 + 0 , 0},
{ 1 * 64 + 1 * 16 + 3 * 4 + 1 , 0},
{ 1 * 64 + 1 * 16 + 3 * 4 + 2 , 0},
{ 1 * 64 + 1 * 16 + 3 * 4 + 3 , 0},
{ 1 * 64 + 2 * 16 + 0 * 4 + 0 , 0},
{ 1 * 64 + 2 * 16 + 0 * 4 + 1 , 0},
{ 1 * 64 + 2 * 16 + 0 * 4 + 2 , 0},
{ 1 * 64 + 2 * 16 + 0 * 4 + 3 , 1},
{ 1 * 64 + 2 * 16 + 1 * 4 + 0 , 0},
{ 1 * 64 + 2 * 16 + 1 * 4 + 1 , 0},
{ 1 * 64 + 2 * 16 + 1 * 4 + 2 , 0},
{ 1 * 64 + 2 * 16 + 1 * 4 + 3 , 0},
{ 1 * 64 + 2 * 16 + 2 * 4 + 0 , 0},
{ 1 * 64 + 2 * 16 + 2 * 4 + 1 , 0},
{ 1 * 64 + 2 * 16 + 2 * 4 + 2 , 0},
{ 1 * 64 + 2 * 16 + 2 * 4 + 3 , 0},
{ 1 * 64 + 2 * 16 + 3 * 4 + 0 , -1},
{ 1 * 64 + 2 * 16 + 3 * 4 + 1 , 0},
{ 1 * 64 + 2 * 16 + 3 * 4 + 2 , 0},
{ 1 * 64 + 2 * 16 + 3 * 4 + 3 , 0},
{ 1 * 64 + 3 * 16 + 0 * 4 + 0 , 0},
{ 1 * 64 + 3 * 16 + 0 * 4 + 1 , 0},
{ 1 * 64 + 3 * 16 + 0 * 4 + 2 , -1},
{ 1 * 64 + 3 * 16 + 0 * 4 + 3 , 0},
{ 1 * 64 + 3 * 16 + 1 * 4 + 0 , 0},
{ 1 * 64 + 3 * 16 + 1 * 4 + 1 , 0},
{ 1 * 64 + 3 * 16 + 1 * 4 + 2 , 0},
{ 1 * 64 + 3 * 16 + 1 * 4 + 3 , 0},
{ 1 * 64 + 3 * 16 + 2 * 4 + 0 , 1},
{ 1 * 64 + 3 * 16 + 2 * 4 + 1 , 0},
{ 1 * 64 + 3 * 16 + 2 * 4 + 2 , 0},
{ 1 * 64 + 3 * 16 + 2 * 4 + 3 , 0},
{ 1 * 64 + 3 * 16 + 3 * 4 + 0 , 0},
{ 1 * 64 + 3 * 16 + 3 * 4 + 1 , 0},
{ 1 * 64 + 3 * 16 + 3 * 4 + 2 , 0},
{ 1 * 64 + 3 * 16 + 3 * 4 + 3 , 0},

{ 2 * 64 + 0 * 16 + 0 * 4 + 0 , 0},
{ 2 * 64 + 0 * 16 + 0 * 4 + 1 , 0},
{ 2 * 64 + 0 * 16 + 0 * 4 + 2 , 0},
{ 2 * 64 + 0 * 16 + 0 * 4 + 3 , 0},
{ 2 * 64 + 0 * 16 + 1 * 4 + 0 , 0},
{ 2 * 64 + 0 * 16 + 1 * 4 + 1 , 0},
{ 2 * 64 + 0 * 16 + 1 * 4 + 2 , 0},
{ 2 * 64 + 0 * 16 + 1 * 4 + 3 , 1},
{ 2 * 64 + 0 * 16 + 2 * 4 + 0 , 0},
{ 2 * 64 + 0 * 16 + 2 * 4 + 1 , 0},
{ 2 * 64 + 0 * 16 + 2 * 4 + 2 , 0},
{ 2 * 64 + 0 * 16 + 2 * 4 + 3 , 0},
{ 2 * 64 + 0 * 16 + 3 * 4 + 0 , 0},
{ 2 * 64 + 0 * 16 + 3 * 4 + 1 , -1},
{ 2 * 64 + 0 * 16 + 3 * 4 + 2 , 0},
{ 2 * 64 + 0 * 16 + 3 * 4 + 3 , 0},
{ 2 * 64 + 1 * 16 + 0 * 4 + 0 , 0},
{ 2 * 64 + 1 * 16 + 0 * 4 + 1 , 0},
{ 2 * 64 + 1 * 16 + 0 * 4 + 2 , 0},
{ 2 * 64 + 1 * 16 + 0 * 4 + 3 , -1},
{ 2 * 64 + 1 * 16 + 1 * 4 + 0 , 0},
{ 2 * 64 + 1 * 16 + 1 * 4 + 1 , 0},
{ 2 * 64 + 1 * 16 + 1 * 4 + 2 , 0},
{ 2 * 64 + 1 * 16 + 1 * 4 + 3 , 0},
{ 2 * 64 + 1 * 16 + 2 * 4 + 0 , 0},
{ 2 * 64 + 1 * 16 + 2 * 4 + 1 , 0},
{ 2 * 64 + 1 * 16 + 2 * 4 + 2 , 0},
{ 2 * 64 + 1 * 16 + 2 * 4 + 3 , 0},
{ 2 * 64 + 1 * 16 + 3 * 4 + 0 , 1},
{ 2 * 64 + 1 * 16 + 3 * 4 + 1 , 0},
{ 2 * 64 + 1 * 16 + 3 * 4 + 2 , 0},
{ 2 * 64 + 1 * 16 + 3 * 4 + 3 , 0},
{ 2 * 64 + 2 * 16 + 0 * 4 + 0 , 0},
{ 2 * 64 + 2 * 16 + 0 * 4 + 1 , 0},
{ 2 * 64 + 2 * 16 + 0 * 4 + 2 , 0},
{ 2 * 64 + 2 * 16 + 0 * 4 + 3 , 0},
{ 2 * 64 + 2 * 16 + 1 * 4 + 0 , 0},
{ 2 * 64 + 2 * 16 + 1 * 4 + 1 , 0},
{ 2 * 64 + 2 * 16 + 1 * 4 + 2 , 0},
{ 2 * 64 + 2 * 16 + 1 * 4 + 3 , 0},
{ 2 * 64 + 2 * 16 + 2 * 4 + 0 , 0},
{ 2 * 64 + 2 * 16 + 2 * 4 + 1 , 0},
{ 2 * 64 + 2 * 16 + 2 * 4 + 2 , 0},
{ 2 * 64 + 2 * 16 + 2 * 4 + 3 , 0},
{ 2 * 64 + 2 * 16 + 3 * 4 + 0 , 0},
{ 2 * 64 + 2 * 16 + 3 * 4 + 1 , 0},
{ 2 * 64 + 2 * 16 + 3 * 4 + 2 , 0},
{ 2 * 64 + 2 * 16 + 3 * 4 + 3 , 0},
{ 2 * 64 + 3 * 16 + 0 * 4 + 0 , 0},
{ 2 * 64 + 3 * 16 + 0 * 4 + 1 , 1},
{ 2 * 64 + 3 * 16 + 0 * 4 + 2 , 0},
{ 2 * 64 + 3 * 16 + 0 * 4 + 3 , 0},
{ 2 * 64 + 3 * 16 + 1 * 4 + 0 , -1},
{ 2 * 64 + 3 * 16 + 1 * 4 + 1 , 0},
{ 2 * 64 + 3 * 16 + 1 * 4 + 2 , 0},
{ 2 * 64 + 3 * 16 + 1 * 4 + 3 , 0},
{ 2 * 64 + 3 * 16 + 2 * 4 + 0 , 0},
{ 2 * 64 + 3 * 16 + 2 * 4 + 1 , 0},
{ 2 * 64 + 3 * 16 + 2 * 4 + 2 , 0},
{ 2 * 64 + 3 * 16 + 2 * 4 + 3 , 0},
{ 2 * 64 + 3 * 16 + 3 * 4 + 0 , 0},
{ 2 * 64 + 3 * 16 + 3 * 4 + 1 , 0},
{ 2 * 64 + 3 * 16 + 3 * 4 + 2 , 0},
{ 2 * 64 + 3 * 16 + 3 * 4 + 3 , 0},

{ 3 * 64 + 0 * 16 + 0 * 4 + 0 , 0},
{ 3 * 64 + 0 * 16 + 0 * 4 + 1 , 0},
{ 3 * 64 + 0 * 16 + 0 * 4 + 2 , 0},
{ 3 * 64 + 0 * 16 + 0 * 4 + 3 , 0},
{ 3 * 64 + 0 * 16 + 1 * 4 + 0 , 0},
{ 3 * 64 + 0 * 16 + 1 * 4 + 1 , 0},
{ 3 * 64 + 0 * 16 + 1 * 4 + 2 , -1},
{ 3 * 64 + 0 * 16 + 1 * 4 + 3 , 0},
{ 3 * 64 + 0 * 16 + 2 * 4 + 0 , 0},
{ 3 * 64 + 0 * 16 + 2 * 4 + 1 , 1},
{ 3 * 64 + 0 * 16 + 2 * 4 + 2 , 0},
{ 3 * 64 + 0 * 16 + 2 * 4 + 3 , 0},
{ 3 * 64 + 0 * 16 + 3 * 4 + 0 , 0},
{ 3 * 64 + 0 * 16 + 3 * 4 + 1 , 0},
{ 3 * 64 + 0 * 16 + 3 * 4 + 2 , 0},
{ 3 * 64 + 0 * 16 + 3 * 4 + 3 , 0},
{ 3 * 64 + 1 * 16 + 0 * 4 + 0 , 0},
{ 3 * 64 + 1 * 16 + 0 * 4 + 1 , 0},
{ 3 * 64 + 1 * 16 + 0 * 4 + 2 , 1},
{ 3 * 64 + 1 * 16 + 0 * 4 + 3 , 0},
{ 3 * 64 + 1 * 16 + 1 * 4 + 0 , 0},
{ 3 * 64 + 1 * 16 + 1 * 4 + 1 , 0},
{ 3 * 64 + 1 * 16 + 1 * 4 + 2 , 0},
{ 3 * 64 + 1 * 16 + 1 * 4 + 3 , 0},
{ 3 * 64 + 1 * 16 + 2 * 4 + 0 , -1},
{ 3 * 64 + 1 * 16 + 2 * 4 + 1 , 0},
{ 3 * 64 + 1 * 16 + 2 * 4 + 2 , 0},
{ 3 * 64 + 1 * 16 + 2 * 4 + 3 , 0},
{ 3 * 64 + 1 * 16 + 3 * 4 + 0 , 0},
{ 3 * 64 + 1 * 16 + 3 * 4 + 1 , 0},
{ 3 * 64 + 1 * 16 + 3 * 4 + 2 , 0},
{ 3 * 64 + 1 * 16 + 3 * 4 + 3 , 0},
{ 3 * 64 + 2 * 16 + 0 * 4 + 0 , 0},
{ 3 * 64 + 2 * 16 + 0 * 4 + 1 , -1},
{ 3 * 64 + 2 * 16 + 0 * 4 + 2 , 0},
{ 3 * 64 + 2 * 16 + 0 * 4 + 3 , 0},
{ 3 * 64 + 2 * 16 + 1 * 4 + 0 , 1},
{ 3 * 64 + 2 * 16 + 1 * 4 + 1 , 0},
{ 3 * 64 + 2 * 16 + 1 * 4 + 2 , 0},
{ 3 * 64 + 2 * 16 + 1 * 4 + 3 , 0},
{ 3 * 64 + 2 * 16 + 2 * 4 + 0 , 0},
{ 3 * 64 + 2 * 16 + 2 * 4 + 1 , 0},
{ 3 * 64 + 2 * 16 + 2 * 4 + 2 , 0},
{ 3 * 64 + 2 * 16 + 2 * 4 + 3 , 0},
{ 3 * 64 + 2 * 16 + 3 * 4 + 0 , 0},
{ 3 * 64 + 2 * 16 + 3 * 4 + 1 , 0},
{ 3 * 64 + 2 * 16 + 3 * 4 + 2 , 0},
{ 3 * 64 + 2 * 16 + 3 * 4 + 3 , 0},
{ 3 * 64 + 3 * 16 + 0 * 4 + 0 , 0},
{ 3 * 64 + 3 * 16 + 0 * 4 + 1 , 0},
{ 3 * 64 + 3 * 16 + 0 * 4 + 2 , 0},
{ 3 * 64 + 3 * 16 + 0 * 4 + 3 , 0},
{ 3 * 64 + 3 * 16 + 1 * 4 + 0 , 0},
{ 3 * 64 + 3 * 16 + 1 * 4 + 1 , 0},
{ 3 * 64 + 3 * 16 + 1 * 4 + 2 , 0},
{ 3 * 64 + 3 * 16 + 1 * 4 + 3 , 0},
{ 3 * 64 + 3 * 16 + 2 * 4 + 0 , 0},
{ 3 * 64 + 3 * 16 + 2 * 4 + 1 , 0},
{ 3 * 64 + 3 * 16 + 2 * 4 + 2 , 0},
{ 3 * 64 + 3 * 16 + 2 * 4 + 3 , 0},
{ 3 * 64 + 3 * 16 + 3 * 4 + 0 , 0},
{ 3 * 64 + 3 * 16 + 3 * 4 + 1 , 0},
{ 3 * 64 + 3 * 16 + 3 * 4 + 2 , 0},
{ 3 * 64 + 3 * 16 + 3 * 4 + 3 , 0}};

std::map<size_t, int> const eta_eval {
{ 0 * 4 + 0 , 1},
{ 0 * 4 + 1 , 0},
{ 0 * 4 + 2 , 0},
{ 0 * 4 + 3 , 0},
{ 1 * 4 + 0 , 0},
{ 1 * 4 + 1 , -1},
{ 1 * 4 + 2 , 0},
{ 1 * 4 + 3 , 0},
{ 2 * 4 + 0 , 0},
{ 2 * 4 + 1 , 0},
{ 2 * 4 + 2 , -1},
{ 2 * 4 + 3 , 0},
{ 3 * 4 + 0 , 0},
{ 3 * 4 + 1 , 0},
{ 3 * 4 + 2 , 0},
{ 3 * 4 + 3 , -1}};

std::vector<Index> const new_dummies_template { Index::a, Index::b, Index::c, Index::d, Index::e,
                                                Index::f, Index::g, Index::h, Index::i, Index::j,
                                                Index::k, Index::l, Index::m, Index::n, Index::o,
                                                Index::p, Index::q, Index::r, Index::s, Index::t,
                                                Index::u, Index::v, Index::w, Index::x, Index::y, Index::z };

template <typename Archive>
void MonomialExpression::serialize(Archive & ar, unsigned int const version) {
  if (version > 0) {
  } else {
  }
  ar & index_mapping;
}

template void MonomialExpression::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive&, unsigned int const);
template void MonomialExpression::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive&, unsigned int const);


bool operator== (IndexMappingEntry const & A, IndexMappingEntry const & B) {
  return (*A.first == *B.first) && (*A.second == *B.second);;
}

bool operator< (IndexMappingEntry const & A, IndexMappingEntry const & B) {
  if (*(A.second) < *(B.second)) {
    return true;
  } else if (*(B.second) < *(A.second)) {
    return false;
  }

  return *(A.first) < *(B.first);
}

MonomialExpression::MonomialExpression() : index_mapping(std::make_unique<IndexMapping>()) { }

MonomialExpression::MonomialExpression(Tensor const & tensor_set, Indices const & indices_set) : index_mapping(std::make_unique<IndexMapping>()) { 
  auto mapping_set = std::make_unique<IndexMapping>(tensor_set.GetIndexMapping(indices_set));
  std::swap(index_mapping, mapping_set);
}

MonomialExpression::MonomialExpression(MonomialExpression const & other) : index_mapping(std::make_unique<IndexMapping>()) { 
  auto vec = std::make_unique<IndexMapping>();

  for (auto & pair : *other.index_mapping) {
    vec->push_back(std::pair<std::unique_ptr<Indices>, std::unique_ptr<Tensor>>());
    auto indices_set = std::make_unique<Indices>(*pair.first);
    auto tensor_set = std::make_unique<Tensor>(*pair.second);
    std::swap((--vec->end())->first, indices_set);
    std::swap((--vec->end())->second, tensor_set);
  }

  std::swap(index_mapping, vec);
}

bool MonomialExpression::IsZero() const {
  return index_mapping->empty();
}

void MonomialExpression::Sort() {
  std::sort(index_mapping->begin(), index_mapping->end());
}

std::pair<std::vector<Index>, std::vector<Index>> MonomialExpression::GetFreeAndDummyIndices() const {
  std::vector<Index> free_indices;
  std::vector<Index> dummy_indices;

  std::for_each(index_mapping->cbegin(), index_mapping->cend(), [&free_indices,&dummy_indices](auto const & a) {
    std::vector<Index> indices { a.first->get_vector() };
    std::for_each(indices.begin(), indices.end(), [&free_indices, &dummy_indices] (auto const & i) {
        if (std::find(dummy_indices.begin(), dummy_indices.end(), i) != dummy_indices.end()) {
        } else if (std::find(free_indices.begin(), free_indices.end(), i) == free_indices.end()) {
          free_indices.push_back(i);
        } else {
          dummy_indices.push_back(i);
          free_indices.erase(std::remove(free_indices.begin(), free_indices.end(), i), free_indices.end());
        }
      });
    });

  return std::make_pair(free_indices, dummy_indices);
}

std::vector<Index> MonomialExpression::GetFreeIndices() const {
  return GetFreeAndDummyIndices().first;
}

std::vector<Index> MonomialExpression::GetDummyIndices() const {
  return GetFreeAndDummyIndices().second;
}

std::vector<Index> MonomialExpression::GetNonFreeIndices() const {
  std::vector<Index> free_indices = GetFreeIndices();

  std::vector<Index> nonfree = new_dummies_template;

  nonfree.erase(std::remove_if(nonfree.begin(), nonfree.end(),
    [&free_indices] (auto & a) {
      return (std::find(free_indices.cbegin(), free_indices.cend(), a) != free_indices.cend());
    }), nonfree.end());

  return nonfree;
}

std::vector<Index> MonomialExpression::GetUnusedIndices() const {
  auto [free_indices, dummy_indices] = GetFreeAndDummyIndices();

  std::vector<Index> unused = new_dummies_template;

  unused.erase(std::remove_if(unused.begin(), unused.end(),
    [&free_indices,&dummy_indices] (auto & a) {
      return (std::find(free_indices.cbegin(), free_indices.cend(), a) != free_indices.cend() || std::find(dummy_indices.cbegin(), dummy_indices.cend(), a) != dummy_indices.cend());
    }), unused.end());

  return unused;
}

bool MonomialExpression::ApplySymmetries() {
  bool even = true;

  for (auto & pair : *index_mapping) {
    if (pair.second->IsSymmetric()) {
      pair.first->Sort(); 
    } else if (pair.second->IsAntisymmetric()) {
      if (!pair.first->Sort()) {
        even = !even;
      }
    }
  }

  return even;
}

Rational MonomialExpression::EvaluateIndices(std::map<Index, size_t> const & evaluation_map) const {
  int ret = 1;
  for (auto & entry : *index_mapping) {
    if (entry.second->get_name() == "eta") {
      int tmp_value = eta_eval.at(   4 * evaluation_map.at(entry.first->at(0))
                                   +     evaluation_map.at(entry.first->at(1))  );
      if (tmp_value == 0) {
        return Rational(0,1);
      } else if (tmp_value == -1) {
        ret = -ret;
      }
    } else if (entry.second->get_name() == "epsilon") {
      int tmp_value =  epsilon_eval.at(   64 * evaluation_map.at(entry.first->at(0))
                                        + 16 * evaluation_map.at(entry.first->at(1))
                                        +  4 * evaluation_map.at(entry.first->at(2))
                                        +      evaluation_map.at(entry.first->at(3)) );

      if (tmp_value == 0) {
        return Rational(0, 1);
      } else if (tmp_value == -1) {
        ret = -ret;
      }

    } else {
      assert(false);
    }
  }

  return Rational(ret, 1);
}

void MonomialExpression::ExchangeIndices(Indices const & indices1, Indices const & indices2) {
  std::map<Index, Index> exchange_map;
  for (size_t index_counter = 0; index_counter < indices1.size(); ++index_counter) {
    exchange_map[indices1.at(index_counter)] = indices2.at(index_counter);
  }

  auto index_mapping_new = std::make_unique<IndexMapping>();
  for (auto & entry : *index_mapping) {
    auto indices_new = std::make_unique<Indices>(*(entry.first), exchange_map);
    index_mapping_new->push_back(std::make_pair(std::make_unique<Indices>(), std::make_unique<Tensor>()));
    std::swap((--(index_mapping_new->end()))->first, indices_new);
    std::swap((--(index_mapping_new->end()))->second, entry.second);
  }

  std::swap(index_mapping, index_mapping_new);
}

void MonomialExpression::ExchangeFreeIndices(Indices const & indices1, Indices const & indices2) {
  auto indices1_v (indices1.get_vector());
  auto indices2_v (indices2.get_vector());

  auto free_indices (GetFreeIndices());
  auto dummy_indices (GetDummyIndices());

  auto new_dummies (new_dummies_template);

  new_dummies.erase(std::remove_if(new_dummies.begin(), new_dummies.end(),
    [&indices2_v] (auto & a) {
      return std::find(indices2_v.begin(), indices2_v.end(), a) != indices2_v.end();
    }), new_dummies.end());

  new_dummies.resize(dummy_indices.size());

  std::vector<Index> indices1_v_total (indices1_v);
  indices1_v_total.reserve(indices1_v.size() + dummy_indices.size());
  std::for_each(dummy_indices.cbegin(), dummy_indices.cend(),
    [&indices1_v_total] (auto const & a) {
      indices1_v_total.emplace_back(a);
    });

  std::vector<Index> indices2_v_total (indices2_v);
  indices2_v_total.reserve(indices2_v.size() + new_dummies.size());
  std::for_each(new_dummies.cbegin(), new_dummies.cend(),
    [&indices2_v_total] (auto const & a) {
      indices2_v_total.emplace_back(a);
    });

  assert(indices1_v_total.size() == indices2_v_total.size());

  ExchangeIndices(Indices(indices1_v_total), Indices(indices2_v_total));
}

void MonomialExpression::AddFactorRight(MonomialExpression const & other) {
  auto deep_copy =
    [this] (auto const & b) mutable {
      this->index_mapping->push_back(std::make_pair(std::make_unique<Indices>(*b.first), std::make_unique<Tensor>(*b.second)));
    };

  std::for_each(other.index_mapping->cbegin(), other.index_mapping->cend(), deep_copy);
}

Status MonomialExpression::ApplySymmetriesToContractions() {
  std::vector<Index> free_indices;
  std::vector<Index> dummy_indices;

  std::for_each(index_mapping->cbegin(), index_mapping->cend(), [&free_indices,&dummy_indices](auto const & a) {
    std::vector<Index> indices { a.first->get_vector() };
    std::for_each(indices.begin(), indices.end(), [&free_indices, &dummy_indices] (auto const & i) {
        if (std::find(dummy_indices.begin(), dummy_indices.end(), i) != dummy_indices.end()) {
        } else if (std::find(free_indices.begin(), free_indices.end(), i) == free_indices.end()) {
          free_indices.push_back(i);
        } else {
          dummy_indices.push_back(i);
          free_indices.erase(std::remove(free_indices.begin(), free_indices.end(), i), free_indices.end());
        }
      });
    });

  std::map<Index, Index> exchange_map;
  bool is_even = true;
  auto it = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [&is_even,&exchange_map,&dummy_indices,this](auto const & a) {
      if ((!a.second->IsSymmetric() && !a.second->IsAntisymmetric()) || a.first->size() < 2 ) {
        return false;
      }

      std::vector<Index> ind_t { a.first->get_vector() };
      std::set<Index> ind_t_set (ind_t.begin(), ind_t.end());
      if (ind_t.size() != ind_t_set.size()) {
        return false;
      }

      Indices overlap ( a.first->Overlap(Indices (dummy_indices) ) );
      std::vector<Index> overlap_vec = overlap.get_vector();
      if (overlap.size() < 2) {
        return false;
      }

      std::vector<Index> dummy_vec;
      std::for_each(index_mapping->cbegin(), index_mapping->cend(),
        [&overlap_vec,&dummy_vec,&a] (auto const & b) {
          if (b.second.get() == a.second.get()) {
            return;
          }
          std::vector<Index> ind_t_2 { b.first->get_vector() };
          std::for_each(ind_t_2.cbegin(), ind_t_2.cend(),
            [&overlap_vec,&dummy_vec,&b] (auto const & i) {
              if (std::find(overlap_vec.cbegin(), overlap_vec.cend(), i) != overlap_vec.cend()) {
                dummy_vec.push_back(i);
              }
            });
        });

      std::vector<Index> dummy_vec_sorted = dummy_vec;
      bool _is_even = Indices::SortIndexVector(dummy_vec_sorted);

      if (dummy_vec == dummy_vec_sorted) {
        return false;
      }

      for (unsigned int counter = 0; counter < dummy_vec.size(); ++counter) {
        exchange_map.insert(std::make_pair(dummy_vec.at(counter), dummy_vec_sorted.at(counter)));
      }

      assert(!(a.second->IsSymmetric() && a.second->IsAntisymmetric()));
      is_even = _is_even || !a.second->IsAntisymmetric();

      return true;
  });

  if (it != index_mapping->cend()) {
    Tensor * sym_tensor = it->second.get();
    std::for_each(index_mapping->begin(), index_mapping->end(), 
      [&exchange_map,sym_tensor] (auto & a) {
        if (a.second.get() == sym_tensor) {
          return;
        } else {
          std::vector<Index> to_replaced = a.first->get_vector();
          std::for_each(to_replaced.begin(), to_replaced.end(),
            [&exchange_map] (auto & a) {
              auto const it = exchange_map.find(a);
              if (it != exchange_map.cend()) {
                a = it->second;
              }
            });
          auto new_indices = std::make_unique<Indices>(to_replaced);
          std::swap(new_indices, a.first);
        }
      });
    return ( is_even ? SYM_EVEN : SYM_ODD );

  } else {
    return NO_ACTION;
  }
}

void MonomialExpression::RenameDummies() {
  std::vector<Index> dummy_indices = GetDummyIndices();

  if (dummy_indices.empty()) {
    return;
  } else {
    std::vector<Index> new_dummies = GetNonFreeIndices();

    std::map<Index, Index> dummy_map;
    for (size_t counter = 0; counter < dummy_indices.size(); ++counter) {
      dummy_map.insert(std::make_pair(dummy_indices.at(counter), new_dummies.at(counter)));
    }

    std::for_each(index_mapping->begin(), index_mapping->end(),
      [&dummy_map] ( auto & a) {
        std::vector<Index> ind_vec = a.first->get_vector();
        std::for_each(ind_vec.begin(), ind_vec.end(),
          [&dummy_map] ( auto & i ) {
            auto it = dummy_map.find(i);
            if (it == dummy_map.end()) {
              return;
            } else {
              i = it->second;
              return;
            }
          });
        auto ind_new = std::make_unique<Indices>(ind_vec);
        std::swap(ind_new, a.first);
      });
  }
}

Status MonomialExpression::EliminateEtaPartial() {
  std::vector<Index> overlap;
  auto it_eta = index_mapping->cend();
  // looking for partial which is contracted with an eta
  auto it_partial = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [this,&it_eta,&overlap](auto & a) {
      if (a.second->get_name() != "partial") {
        return false;
      }
  // looking an eta which is contracted with our partial
      it_eta = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
        [&a,&overlap,&it_eta](auto & b) {
          if (b.second->get_name() != "eta" || b.first->size() != 2) {
            return false;
          }
          overlap = a.first->Overlap(*b.first).get_vector();
          return overlap.size() > 0;
      });
      if (it_eta == index_mapping->cend()) {
        return false;
      } else {
        return true;
      }
    });
  if (it_eta == index_mapping->cend() || it_partial == index_mapping->cend()) {
    // nothing found, returning
    return NO_ACTION;
  } else {
    // We found a contracted pair of partial and eta!
    // Now we define a new index mapping and fill it with the contents of our old mapping,
    // except that we neglect our eta and replace our partial.
    auto new_mapping = std::make_unique<IndexMapping>();
    for (auto it = index_mapping->cbegin(); it < index_mapping->cend(); ++it) {
      if (it == it_eta) {
        continue;
      } else if (it == it_partial) {
        if (overlap.size() == 1) {
          std::vector<Index> indices_partial = it->first->get_vector();
          std::vector<Index> indices_eta = it_eta->first->get_vector();
          Index new_index;
          if (overlap.at(0) == indices_eta.at(0)) {
            new_index = indices_eta.at(1);
          } else if (overlap.at(0) == indices_eta.at(1)) {
            new_index = indices_eta.at(0);
          } else {
            assert(false);
          }
          std::replace(indices_partial.begin(), indices_partial.end(), overlap.at(0), new_index);
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>(indices_partial), std::make_unique<Tensor>(*(it->second))));
        } else if (overlap.size() == 2) {
          std::vector<Index> indices_partial = it->first->get_vector();
          indices_partial.erase(std::remove_if(indices_partial.begin(), indices_partial.end(),
            [&overlap](auto const & a) {
              if (std::find(overlap.cbegin(), overlap.cend(), a) == overlap.cend()) {
                return false;
              } else {
                return true;
              }
            }), indices_partial.end());
          Tensor new_partial (it->second->get_rank() - 1, "partial");
          if (new_partial.get_rank() > 1) {
            new_partial.SetSymmetric();
          }
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>(indices_partial), std::make_unique<Tensor>(new_partial)));
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>(), std::make_unique<Tensor>(0, "partialBox")));
        } else {
          assert(false);
        }
      } else {
        new_mapping->push_back(std::make_pair(std::make_unique<Indices>(*(it->first)), std::make_unique<Tensor>(*(it->second))));
      }
    }
    std::swap(index_mapping, new_mapping);
    return ETA_PARTIAL_CONTRACTION;
  }
}

Status MonomialExpression::EliminateEtaRankOne() {
  Index overlap;
  auto it_eta = index_mapping->cend();
  // looking for rank one tensor which is contracted with an eta
  auto it_tensor = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [this,&it_eta,&overlap](auto & a) {
      if (a.first->size() != 1) {
        return false;
      }
  // looking an eta which is contracted with our tensor 
      it_eta = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
        [&a,&overlap,&it_eta](auto & b) {
          if (b.second->get_name() != "eta" || b.first->size() != 2) {
            return false;
          }
          auto overlap_vec = a.first->Overlap(*b.first).get_vector();
          if (overlap_vec.size() == 1) {
            overlap = overlap_vec.at(0);
            return true;
          } else {
            return false;
          }
      });
      if (it_eta == index_mapping->cend()) {
        return false;
      } else {
        return true;
      }
    });
  if (it_eta == index_mapping->cend() || it_tensor == index_mapping->cend()) {
    // nothing found, returning
    return NO_ACTION;
  } else {
    // We found a contracted pair of tensor and eta!
    // Now we define a new index mapping and fill it with the contents of our old mapping,
    // except that we neglect our eta and replace our tensor.
    auto new_mapping = std::make_unique<IndexMapping>();
    for (auto it = index_mapping->cbegin(); it < index_mapping->cend(); ++it) {
      if (it == it_eta) {
        continue;
      } else if (it == it_tensor) {
        std::vector<Index> indices_eta = it_eta->first->get_vector();
        Index new_index;
        if (overlap == indices_eta.at(0)) {
          new_index = indices_eta.at(1);
        } else if (overlap == indices_eta.at(1)) {
          new_index = indices_eta.at(0);
        } else {
          new_index = Index::z;
          assert(false);
        }
        new_mapping->push_back(std::make_pair(std::make_unique<Indices>(Indices({new_index})), std::make_unique<Tensor>(*(it->second))));
      } else {
        new_mapping->push_back(std::make_pair(std::make_unique<Indices>(*(it->first)), std::make_unique<Tensor>(*(it->second))));
      }
    }
    std::swap(index_mapping, new_mapping);
    return ETA_CONTRACTION;
  }
}

Status MonomialExpression::EliminateEpsilon(unsigned int const dimension) {
  auto it_epsilon = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [this,dimension](auto & a) {
      if (!(a.second->get_name() == "epsilon" || a.second->get_name() == "epsilonI") || a.second->get_rank() != dimension || !a.second->IsAntisymmetric()) {
        return false;
      }
      Indices epsilon_indices = *a.first;
      epsilon_indices.SortAndMakeUnique();
      if (epsilon_indices.size() < dimension) {
        return true;
      }
      auto it_other = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
        [&a](auto & b) {
          if (b.second->IsSymmetric()) {
            Indices overlap = b.first->Overlap(*a.first);
            if (overlap.size() > 1) {
              return true;
            } else {
              return false;
            }
          } else {
            return false;
          }
        });
      if (it_other == index_mapping->cend()) {
        return false;
      } else {
        return true;
      }
    });

  return (it_epsilon == index_mapping->cend() ? NO_ACTION : EPSILON_TO_ZERO);
}

Status MonomialExpression::EliminateEpsilonI() {
  auto it_epsilonI = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [](auto const & a) {
      return ((a.second->get_name() == "epsilonI") && (a.second->IsAntisymmetric()) && (a.first->size() == 4));
    });

  if (it_epsilonI == index_mapping->cend()) {
    return NO_ACTION;
  } else {
    std::vector<Index> unused_indices = GetUnusedIndices();
    Tensor eta (2, "eta");
    eta.SetSymmetric();

    Tensor epsilon (4, "epsilon");
    epsilon.SetAntisymmetric();

    auto new_mapping = std::make_unique<IndexMapping>();
    std::for_each(index_mapping->cbegin(), index_mapping->cend(),
      [&new_mapping,&it_epsilonI,unused_indices,eta,epsilon] (auto const & a){
        if ( a != *it_epsilonI ) {
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>(*a.first), std::make_unique<Tensor>(*a.second)));
        } else {
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>( Indices({ unused_indices.at(0), unused_indices.at(1), unused_indices.at(2), unused_indices.at(3) })),
                                                std::make_unique<Tensor>(epsilon)));
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>( Indices({ unused_indices.at(0), it_epsilonI->first->at(0) }) ), std::make_unique<Tensor>(eta)));
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>( Indices({ unused_indices.at(1), it_epsilonI->first->at(1) }) ), std::make_unique<Tensor>(eta)));
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>( Indices({ unused_indices.at(2), it_epsilonI->first->at(2) }) ), std::make_unique<Tensor>(eta)));
          new_mapping->push_back(std::make_pair(std::make_unique<Indices>( Indices({ unused_indices.at(3), it_epsilonI->first->at(3) }) ), std::make_unique<Tensor>(eta)));
        }
      });
    std::swap(index_mapping, new_mapping);
    return EPSILONI;
  }
}

std::pair<Indices, Indices> MonomialExpression::EliminateEpsilonEpsilonI(unsigned int const dimension) {
  auto it_epsilon = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [dimension](auto const & a) {
      return (a.second->get_name() == "epsilon" && a.second->IsAntisymmetric() && a.first->size() == dimension);
    });

  if (it_epsilon == index_mapping->cend()) {
    return std::make_pair(Indices { }, Indices { });
  }

  auto it_epsilonI = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [&it_epsilon,dimension](auto const & a) {
      return ((&a != &(*it_epsilon)) && a.second->get_name() == (dimension == 4 ? "epsilonI" : "epsilon") && a.second->IsAntisymmetric() && a.first->size() == dimension);
    });
  
  if (it_epsilonI == index_mapping->cend()) {
    return std::make_pair(Indices { }, Indices { });
  } else {
    auto epsilon = std::make_pair(std::make_unique<Indices>(*(it_epsilon->first)), std::make_unique<Tensor>(*(it_epsilon->second)));
    auto epsilonI = std::make_pair(std::make_unique<Indices>(*(it_epsilonI->first)), std::make_unique<Tensor>(*(it_epsilonI->second)));
    
    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&epsilon,&epsilonI] (auto const & a) {
          return (epsilon == a || epsilonI == a);
        }
      ), index_mapping->end());

    return std::make_pair(*(epsilon.first), *(epsilonI.first));
  }
}

Status MonomialExpression::EliminateGammaGamma() {
  return EliminateMetricMetric(3);
}

Status MonomialExpression::EliminateEtaEta() {
  return EliminateMetricMetric(4);
}

Status MonomialExpression::EliminateMetricMetric(unsigned int const dimension) {
  std::pair<Tensor *, Tensor *> to_eliminate = std::make_pair(nullptr, nullptr);
  Indices overlap;

  bool break_now = false;

  for (auto it1 = index_mapping->cbegin(); it1 < index_mapping->cend(); ++it1) {
    for (auto it2 = it1 + 1; it2 < index_mapping->cend(); ++it2) {
      if (it1->second->get_name() == ( dimension == 4 ? "eta" : "gamma") && it2->second->get_name() == ( dimension == 4 ? "eta" : "gamma")) {
        Indices _overlap = it1->first->Overlap(*(it2->first));

        if (_overlap.size() > 0) {
          to_eliminate = std::make_pair(it1->second.get(), it2->second.get());
          overlap = _overlap;
          break_now = true;
          break;
        }
      }
    }
    if (break_now) {
      break;
    }
  }

  if (overlap.size() == 0) {
    return NO_ACTION;
  } else if (overlap.size() > 2) {
    assert(false);
    return ERROR;
  }

  if (overlap.size() == 1) {
    Tensor new_metric(2, (dimension == 4 ? "delta" : "gamma"));
    new_metric.SetSymmetric();

    std::vector<Index> indices_new;

    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&indices_new,&overlap,&to_eliminate] (auto const & a) {
          if (a.second.get() == to_eliminate.first || a.second.get() == to_eliminate.second) {
            if (a.first->at(0) != overlap.at(0)) {
              indices_new.push_back(a.first->at(0));
            } else if (a.first->at(1) != overlap.at(0)) {
              indices_new.push_back(a.first->at(1));
            } else {
              assert(false);
            }
            return true;
          } else {
            return false;
          }
        }),
      index_mapping->end());

    assert (indices_new.size() == 2);

    this->AddFactorRight(MonomialExpression(new_metric, Indices({indices_new.at(0), indices_new.at(1)})));

    return (dimension == 4 ? ETA_ETA_TO_DELTA : GAMMA_GAMMA_TO_DELTA);
  }



  if (overlap.size() == 2) {
    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&to_eliminate] (auto const & a) {
          return (a.second.get() == to_eliminate.first || a.second.get() == to_eliminate.second);
        }),
      index_mapping->end());

    return (dimension == 4 ? ETA_ETA_TO_TRACE : GAMMA_GAMMA_TO_TRACE);
  } 
  assert(false);
  return ERROR;
}

Status MonomialExpression::EliminateGamma() {
  return EliminateDelta("gamma");
}

Status MonomialExpression::EliminateTracefree() {
  if (
    std::find_if(index_mapping->cbegin(), index_mapping->cend(),
      [] (auto const & a) {
        return (a.second->IsTracefree() && a.first->at(0) == a.first->at(1));
      }) != index_mapping->cend()
    ) {
    return TRACE_TO_ZERO;
  } else {
    return NO_ACTION;
  }
}

Status MonomialExpression::EliminateDelta(std::string const & delta_name) {

  auto it_other = index_mapping->cend();

  auto it_delta = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
    [&delta_name,&it_other,this] (auto const & a) {

    if (!(a.second->get_name() == delta_name && a.second->get_rank() == 2)) {
      return false;
    } else if (a.first->at(0) == a.first->at(1)) {
      return true;
    } else {
      Indices overlap;
      it_other = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
        [&a,&overlap] (auto const & b) {
          if (&b == &(a)) {
            return false;
          } else {
            overlap = b.first->Overlap(*(a.first));
            return (overlap.size() > 0);
          }
        });
  
      if (it_other == index_mapping->cend()) {
        return false;
      } else {
        return true;
      }
    }

    });

  if (it_delta == index_mapping->cend()) {
    return NO_ACTION;
  } else if (it_delta->first->at(0) == it_delta->first->at(1)) {
    index_mapping->erase(std::remove_if(index_mapping->begin(), index_mapping->end(),
      [&it_delta] (auto const & a) {
        return (&a == &*it_delta);
      }), index_mapping->end());
    return DELTA_TO_TRACE;
  } else if (it_other != index_mapping->cend() && it_delta != it_other) {
    Indices overlap = it_delta->first->Overlap(*(it_other->first));
    
    if (overlap.size() == 2) {
      it_other->first->Replace(overlap.at(1), overlap.at(0));
    } else {
      if (overlap.at(0) == it_delta->first->at(0)) {
        it_other->first->Replace(overlap.at(0), it_delta->first->at(1));
      } else {
        it_other->first->Replace(overlap.at(0), it_delta->first->at(0));
      }
    }
    index_mapping->erase(std::remove_if(index_mapping->begin(), index_mapping->end(),
      [&it_delta] (auto const & a) {
        return (&a == &(*it_delta));
      }), index_mapping->end());
    return DELTA_OK;
  } else {
    return NO_ACTION;
  }

/*
  if (delta_name == "gamma") {
    std::for_each(index_mapping->begin(), index_mapping->end(),
      [] (auto & a) {
        if (a.second->get_name() == "delta") {
          auto new_tensor = std::make_unique<Tensor>(2, "gamma");
          new_tensor->SetSymmetric();
          std::swap(a.second, new_tensor);
        }
      });
  }
*/

  return NO_ACTION;

}

Status MonomialExpression::ThreePlusOne(std::vector<Index> indices_to_zero) {
  Indices _indices_to_zero(indices_to_zero);
  int factor = 1;

  index_mapping->erase(std::remove_if(index_mapping->begin(), index_mapping->end(),
    [&_indices_to_zero,&indices_to_zero,&factor](auto & a) {
      if (a.second->get_name() == "eta" && a.second->IsSymmetric() && a.first->size() == 2) {
        Indices overlap = _indices_to_zero.Overlap(*a.first);
        if (overlap.size() == 1) {
          factor *= 0;
          return false;
        } else if (overlap.size() == 0) {
          auto gamma = std::make_unique<Tensor>(2, "gamma");
          gamma->SetSymmetric();
          std::swap(a.second, gamma);
          factor *= -1;
          return false;
        } else if (overlap.size() == 2) {
          return true;
        } else {
          assert(false);
          return false;
        }
      } else if (a.second->get_name() == "epsilon" && a.second->IsAntisymmetric() && a.first->size() == 4) {
        Indices overlap = _indices_to_zero.Overlap(*a.first);
        if (overlap.size() == 0) {
          factor *= 0;
          return false;
        } else if (overlap.size() == 1) {
          std::vector<Index> epsilon_indices = a.first->get_vector();
          auto it = std::find(epsilon_indices.cbegin(), epsilon_indices.cend(), overlap.at(0));
          if (std::distance(it, epsilon_indices.cbegin()) % 2 == 0) {
            factor *= -1;
          }
          auto _new_indices = std::make_unique<std::vector<Index>> ();
          std::for_each(epsilon_indices.cbegin(), epsilon_indices.cend(),
            [&_new_indices,&overlap] (auto const & a) {
              if (a != overlap.at(0)) {
                _new_indices->push_back(a);
              }
            });
          auto new_indices = std::make_unique<Indices>(*_new_indices);
          auto new_epsilon = std::make_unique<Tensor>(3,"epsilon");
          new_epsilon->SetAntisymmetric();
          std::swap(a.first, new_indices);
          std::swap(a.second, new_epsilon);
          return false;
        } else if (overlap.size() > 1 && overlap.size() < 5) {
          factor *= 0;
          return false;
        } else {
          assert(false);
          return false;
        }
      } else if (a.second->get_name() == "epsilonI" && a.second->IsAntisymmetric() && a.first->size() == 4) {
        Indices overlap = _indices_to_zero.Overlap(*a.first);
        if (overlap.size() == 0) {
          factor *= 0;
          return false;
        } else if (overlap.size() == 1) {
          std::vector<Index> epsilon_indices = a.first->get_vector();
          auto it = std::find(epsilon_indices.cbegin(), epsilon_indices.cend(), overlap.at(0));
          if (std::distance(it, epsilon_indices.cbegin()) % 2 != 0) {
            factor *= -1;
          }
          auto _new_indices = std::make_unique<std::vector<Index>> ();
          std::for_each(epsilon_indices.cbegin(), epsilon_indices.cend(),
            [&_new_indices,&overlap] (auto const & a) {
              if (a != overlap.at(0)) {
                _new_indices->push_back(a);
              }
            });
          auto new_indices = std::make_unique<Indices>(*_new_indices);
          auto new_epsilon = std::make_unique<Tensor>(3,"epsilon");
          new_epsilon->SetAntisymmetric();
          std::swap(a.first, new_indices);
          std::swap(a.second, new_epsilon);
          return false;
        } else if (overlap.size() > 1 && overlap.size() < 5) {
          factor *= 0;
          return false;
        } else {
          assert(false);
          return false;
        }
      } else {
        return false;
      }
    }), index_mapping->end());

  if (factor == 0) {
    return ZERO_ZERO;
  } else if (factor < 0) {
    return ZERO_NEGATIVE;
  } else {
    return ZERO_POSITIVE;
  }
}

MonomialExpression MonomialExpression::MultiplyOther(MonomialExpression const & other) const {
  MonomialExpression ret;
  auto deep_copy =
    [&ret] (auto const & b) mutable {
      ret.index_mapping->push_back(std::make_pair(std::make_unique<Indices>(*b.first), std::make_unique<Tensor>(*b.second)));
    };

  std::for_each(this->index_mapping->cbegin(), this->index_mapping->cend(), deep_copy);
  std::for_each(other.index_mapping->cbegin(), other.index_mapping->cend(), deep_copy);

  return ret;
}

std::string MonomialExpression::GetLatexString(bool upper) const {
  if (IsZero()) {
    return "0";
  }

  std::stringstream ss;
  bool first = true;

  for (auto & pair : *index_mapping) {
    if (!first) {
      ss << " ";
    }
    first = false;
    ss << pair.second->get_name();
    if (pair.first->size() != 0) {
      ss << (upper ? "^" : "_") << pair.first->ToString();
    }
  }

  return ss.str();
}

bool MonomialExpression::CompareReversed(MonomialExpression const & lop, MonomialExpression const & rop) {
  IndexMapping lop_rev;
  IndexMapping rop_rev;

  lop_rev.reserve(lop.index_mapping->size());
  rop_rev.reserve(rop.index_mapping->size());

  std::for_each(lop.index_mapping->rbegin(), lop.index_mapping->rend(),
    [&lop_rev] (auto const & a) {
      lop_rev.emplace_back(std::make_pair(std::make_unique<Indices>(*a.first), std::make_unique<Tensor>(*a.second)));
    });

  std::for_each(rop.index_mapping->rbegin(), rop.index_mapping->rend(),
    [&rop_rev] (auto const & a) {
      rop_rev.emplace_back(std::make_pair(std::make_unique<Indices>(*a.first), std::make_unique<Tensor>(*a.second)));
    });

  return lop_rev < rop_rev;
}

bool MonomialExpression::operator== (MonomialExpression const & other) const { return *index_mapping == *other.index_mapping; }
bool MonomialExpression::operator!= (MonomialExpression const & other) const { return !(*this == other); }
bool MonomialExpression::operator< (MonomialExpression const & other) const { return *index_mapping < *other.index_mapping; }

