#include "search.h"
#include "evaluate.h"
#include "types.h"
#include <iostream>

int negaMax(int depth, const Position& pos, int maxdepth) {
  bool verbose {false};

  if (depth == 0)
  {
    int factor {color_factor(pos.active_color)};
    return factor*evaluate(pos);
  }

  if (depth > maxdepth)
    maxdepth = depth;

  int maxi = -static_cast<int>(Value::king);
  Move best_move;
  for (auto mov : pos.all_moves()) {
    if (verbose)
      std::cout << "considering move " << move_to_string(mov) << "\n";
    Position pos_copy {pos};
    pos_copy.move(mov);
    int score = -negaMax(depth-1, pos_copy, maxdepth);
    if (score > maxi) {
      maxi = score;
      best_move = mov;
    }
  }
  if (depth==maxdepth)
    std::cout << "best move:" << move_to_string(best_move) << "\n";
  return maxi;
}
