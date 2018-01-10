#include "MonomialExpression.h"

#include <iostream>

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

void MonomialExpression::AddFactorRight(MonomialExpression const & other) {
  auto deep_copy =
    [this] (auto const & b) mutable {
      this->index_mapping->push_back(std::make_pair(std::make_unique<Indices>(*b.first), std::make_unique<Tensor>(*b.second)));
    };

  std::for_each(other.index_mapping->cbegin(), other.index_mapping->cend(), deep_copy);
}

EliminateReturn MonomialExpression::EliminateEpsilon() {
  Tensor * epsilon = nullptr;
  Indices epsilon_indices;

  for (auto const & a : *index_mapping) {
    if (a.second->get_name() == "epsilon" && a.second->get_rank() == 4) {
      epsilon = a.second.get();
      epsilon_indices = *a.first;
      break;
    }
  }

  if (epsilon == nullptr) {
    return NO_ACTION;
  } else {
    epsilon_indices.SortAndMakeUnique();
    if (epsilon_indices.size() < 4) {
      return EPSILON_TO_ZERO;
    }

    for (auto const & a : *index_mapping) {
      if (epsilon == a.second.get()) {
        continue;
      }
      if (a.second->IsSymmetric()) {
        Indices overlap = epsilon_indices.Overlap(*a.first);
        if (overlap.size() > 1) {
          return EPSILON_TO_ZERO;
        }
      }
    }

    return NO_ACTION;
  }
  
}

EliminateReturn MonomialExpression::EliminateEtaEta() {
  std::pair<Tensor *, Tensor *> to_eliminate = std::make_pair(nullptr, nullptr);
  Indices overlap;

  bool break_now = false;

  for (auto it1 = index_mapping->cbegin(); it1 < index_mapping->cend(); ++it1) {
    for (auto it2 = it1 + 1; it2 < index_mapping->cend(); ++it2) {
      if (it1->second->get_name() == "eta" && it2->second->get_name() == "eta") {
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
    Tensor delta(2, "delta");
    delta.SetSymmetric();

    std::vector<Index> indices_delta;

    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&indices_delta,&overlap,&to_eliminate] (auto const & a) {
          if (a.second.get() == to_eliminate.first || a.second.get() == to_eliminate.second) {
            if (a.first->at(0) != overlap.at(0)) {
              indices_delta.push_back(a.first->at(0));
            } else if (a.first->at(1) != overlap.at(0)) {
              indices_delta.push_back(a.first->at(1));
            } else {
              assert(false);
            }
            return true;
          } else {
            return false;
          }
        }),
      index_mapping->end());

    assert (indices_delta.size() == 2);

    this->AddFactorRight(MonomialExpression(delta, Indices({indices_delta.at(0), indices_delta.at(1)})));

    return ETA_ETA_TO_DELTA;
  }



  if (overlap.size() == 2) {
    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&to_eliminate] (auto const & a) {
          return (a.second.get() == to_eliminate.first || a.second.get() == to_eliminate.second);
        }),
      index_mapping->end());

    return ETA_ETA_TO_TRACE;
  } 

  return ERROR;
}

EliminateReturn MonomialExpression::EliminateDelta() {
  Tensor * delta = nullptr;
  Indices delta_indices;

  for (auto const & a : *index_mapping) {
    if (a.second->get_name() == "delta" && a.second->get_rank() == 2) {
      delta = a.second.get();
      delta_indices = *a.first;
      break;
    }
  }

  if (delta == nullptr) {
    return NO_ACTION;
  } else if (delta_indices.at(0) == delta_indices.at(1)) {
    index_mapping->erase(
      std::remove_if(index_mapping->begin(), index_mapping->end(),
        [&delta] (auto const & a) {
          return delta == a.second.get();
        }),
      index_mapping->end());

    return DELTA_TO_TRACE;
  } else {
    auto it = std::find_if(index_mapping->cbegin(), index_mapping->cend(),
                             [&delta,&delta_indices] (auto const & a) {
                               Indices overlap = delta_indices.Overlap(*a.first);
                               return (delta != a.second.get() && overlap.size() > 0);
                             });

    if (it == index_mapping->end()) {
      return NO_ACTION;
    }

    Indices overlap = delta_indices.Overlap(*(it->first));

    if (overlap.size() == 2) {
      it->first->Replace(overlap.at(1), overlap.at(0));

      index_mapping->erase(
        std::remove_if(index_mapping->begin(), index_mapping->end(),
          [&delta] (auto const & a) {
            return delta == a.second.get();
          }),
        index_mapping->end());

      return DELTA_OK;
    } else if (overlap.size() == 1) {
      Index to_replace_with;
      
      if (delta_indices.at(0) != overlap.at(0)) {
        to_replace_with = delta_indices.at(0);
      } else if (delta_indices.at(1) != overlap.at(0)) {
        to_replace_with = delta_indices.at(1);
      } else {
        assert(false);
        return ERROR;
      }

      it->first->Replace(overlap.at(0), to_replace_with);

      index_mapping->erase(
        std::remove_if(index_mapping->begin(), index_mapping->end(),
          [&delta] (auto const & a) {
            return delta == a.second.get();
          }),
        index_mapping->end());

      return DELTA_OK;
    } else {
      assert(false);
      return ERROR;
    }
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
    ss << pair.second->get_name() << (upper ? "^" : "_") << pair.first->ToString();
  }

  return ss.str();
}

bool MonomialExpression::operator== (MonomialExpression const & other) const { return *index_mapping == *other.index_mapping; }
bool MonomialExpression::operator!= (MonomialExpression const & other) const { return !(*this == other); }
bool MonomialExpression::operator< (MonomialExpression const & other) const { return *index_mapping < *other.index_mapping; }

