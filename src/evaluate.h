#ifndef EVALUATE_H
#define EVALUATE_H

#include "position.h"
#include "types.h"
#include <vector>
using std::vector;

enum class Value {
  empty, pawn = 1, knight = 3, bishop = 3, rook = 5, queen = 9, king = 1000
};

const vector<int> pawnRankValue {1, 1, 2, 2, 3, 5, 9};

int find_value(const PieceType& type);
int signed_value(const Piece& pc);
int sum_piece_values(const vector<Piece>& pcs);
int advance_value(const Position& pos, const Color col);

int evaluate(const Position& pos);

#endif
