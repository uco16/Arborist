#ifndef SEARCH_H
#define SEARCH_H

#include "position.h"

const int INT_MAX = (~0u)>>1;
const int INT_MIN = ~INT_MAX;

int negaMax(int depth, Position pos, bool verbose=false, int maxdepth=0, int best_at_previous_depth=INT_MIN);

#endif
