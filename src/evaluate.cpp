#include "evaluate.h"

int find_value(const PieceType& type)
{
  if      (type==PieceType::pawn)     return static_cast<int>(Value::pawn);
  else if (type==PieceType::knight)   return static_cast<int>(Value::knight);
  else if (type==PieceType::bishop)   return static_cast<int>(Value::bishop);
  else if (type==PieceType::rook)     return static_cast<int>(Value::rook);
  else if (type==PieceType::queen)    return static_cast<int>(Value::queen);
  else				      return static_cast<int>(Value::king);
}

int signed_value(const Piece& pc)
{
  int factor = 1-(2*static_cast<int>(color_of(pc))); // maps black to -1 and white to 1
  return factor*find_value(type_of(pc));
}

int sum_piece_values(const vector<Piece>& pcs)
{
  int val {0};
  for (Piece pc: pcs)
    val += signed_value(pc);
  return val;
}

int evaluate(const Position& pos)
// return values:
// 0: equality for black and white
// +ve: white advantage, -ve: black advantage
{
  // find piece value
  int piece_value = sum_piece_values(pos.all_pieces());

  return piece_value;
}
