#ifndef EVALUATE_H
#define EVALUATE_H

#include "position.h"
#include "types.h"
#include <vector>
using std::vector;

enum class Value {
  empty, pawn = 1, knight = 3, bishop = 3, rook = 5, queen = 9, king = 1000
};

int find_value(const PieceType& type);
int signed_value(const Piece& pc);
int sum_piece_values(const vector<Piece>& pcs);

int evaluate(const Position& pos);

#endif
