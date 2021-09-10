#include "search.h"
#include "evaluate.h"
#include "types.h"
#include <iostream>

int negaMax(int depth, Position pos, bool verbose, int maxdepth, int best_at_previous_depth) {

  if (depth > maxdepth)
    maxdepth = depth;

  string tabs {""};
  for (int i=0; i<maxdepth-depth; i++)
    tabs += " | ";

  if (verbose)
    std::cout << tabs << "Depth: " << (maxdepth-depth+1) << ", " << pos.active_color << " to move.\n";

  if (depth == 0)
  {
    int eval {evaluate(pos)};
    if (verbose)
      std::cout << tabs << " | Evaluation: " << eval << "\n";
    return color_factor(pos.active_color)*eval;
  }

  int maxi = INT_MIN;
  Move best_move;

  vector<Move> all_moves {pos.all_moves()};
  if (all_moves.size()==0) {
    if (verbose)
      std::cout << tabs << " | Checkmate in position " << pos.fen() << "\n";
    return (pos.active_color==Color::white ? (INT_MIN+1) : INT_MAX);
  }
  for (auto mov : all_moves) {
    if (verbose)
      std::cout << tabs << " | Move: " << mov << "\n";

    Square enpassant_sq {pos.enpassant()};
    Piece captured_piece {pos.get(mov.fsq)};
    pos.move(mov);
    int score = -negaMax(depth-1, pos, verbose, maxdepth, maxi);
    pos.unmove(mov, captured_piece, enpassant_sq);

    if (verbose)
      std::cout << tabs << " | NegaMax score: " << score;

    if (best_at_previous_depth > INT_MIN && score >= -1*best_at_previous_depth) {
      if (verbose)
	std::cout << ">=" << -1*best_at_previous_depth << ", can prune.\n";
      return score;
    }

    if (score > maxi) {
      maxi = score;
      best_move = mov;
      if (verbose)
	std::cout << ", new best!";
    }
    if (verbose)
      std::cout << "\n";
  }

  if (depth==maxdepth)
    std::cout << tabs << "best move: " << best_move << "\n";
  return maxi;
}
