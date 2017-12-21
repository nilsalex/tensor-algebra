#include "MonomialExpression.h"

#include <algorithm>
#include <cassert>
#include <sstream>

std::map<size_t, int> epsilon_eval {
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

std::map<size_t, int> eta_eval {
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
};

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
};

bool MonomialExpression::IsZero() const {
  return index_mapping->empty();
}

void MonomialExpression::Sort() {
  std::sort(index_mapping->begin(), index_mapping->end());
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
  int tensor_counter = 0;
  for (auto & entry : *index_mapping) {
    if (entry.second->get_name() == "eta") {
      int tmp_value = eta_eval[   4 * evaluation_map.at(entry.first->at(0))
                                +     evaluation_map.at(entry.first->at(1))  ];
      if (tmp_value == 0) {
        return Rational(0,1);
      } else if (tmp_value == -1) {
        ret = -ret;
      }
    } else if (entry.second->get_name() == "epsilon") {
      int tmp_value =  epsilon_eval[   64 * evaluation_map.at(entry.first->at(0))
                                     + 16 * evaluation_map.at(entry.first->at(1))
                                     +  4 * evaluation_map.at(entry.first->at(2))
                                     +      evaluation_map.at(entry.first->at(3)) ];

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
    ss << pair.second->get_name() << (upper ? "^" : "_") << pair.first->ToString();
  }

  return ss.str();
}

bool MonomialExpression::operator== (MonomialExpression const & other) const { return *index_mapping == *other.index_mapping; }
bool MonomialExpression::operator!= (MonomialExpression const & other) const { return !(*this == other); }
bool MonomialExpression::operator< (MonomialExpression const & other) const { return *index_mapping < *other.index_mapping; }

