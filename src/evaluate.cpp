#include "evaluate.h"

int material_value(const Position& pos) {
  int val {0};

  // pawns
  const int num_white_pawns = bitcount(pos.bb_white() & pos.bb_pawns());
  const int num_black_pawns = bitcount(pos.bb_black() & pos.bb_pawns());
  val += static_cast<int>(Value::pawn) * (num_white_pawns - num_black_pawns);

  // knights
  const int num_white_knights = bitcount(pos.bb_white() & pos.bb_knights());
  const int num_black_knights = bitcount(pos.bb_black() & pos.bb_knights());
  val += static_cast<int>(Value::knight) * (num_white_knights - num_black_knights);

  // bishops
  const int num_white_bishops = bitcount(pos.bb_white() & pos.bb_bishops());
  const int num_black_bishops = bitcount(pos.bb_black() & pos.bb_bishops());
  val += static_cast<int>(Value::bishop) * (num_white_bishops - num_black_bishops);
  
  // rooks
  const int num_white_rooks = bitcount(pos.bb_white() & pos.bb_rooks());
  const int num_black_rooks = bitcount(pos.bb_black() & pos.bb_rooks());
  val += static_cast<int>(Value::rook) * (num_white_rooks - num_black_rooks);

  // queens
  const int num_white_queens = bitcount(pos.bb_white() & pos.bb_queens());
  const int num_black_queens = bitcount(pos.bb_black() & pos.bb_queens());
  val += static_cast<int>(Value::queen) * (num_white_queens - num_black_queens);
  
  // kings
  const int num_white_kings = bitcount(pos.bb_white() & pos.bb_kings());
  const int num_black_kings = bitcount(pos.bb_black() & pos.bb_kings());
  val += static_cast<int>(Value::king) * (num_white_kings - num_black_kings);

  return val;
}


int advance_value(const Bitboard pawn_bb, const Color color)
{
  int val {0};
  // There are never any pawns on the first or the last rank
  // when promotion is implemented. For now, we let the engine think of
  // a pawn on the promotion rank as the value of a queen.
  for (int i=0; i<64; i++)
  {
    if (getBit(pawn_bb, 63-i))
    {
      Rank pawn_rank {7-i/8};
      int distance_from_base = abs((base_rank(color)-pawn_rank).value);
      //std::cout << "pawn at " << i << " (rank " << pawn_rank.index << ") is " << distance_from_base << " from base " << base_rank(color).index << "\n";
      val += (pawnRankValue[distance_from_base]-1);
    }
  }
  //std::cout << color_to_char(color) << " adv. value: " << val << "\n";
  return val;
}

int evaluate(const Position& pos)
// return values:
// 0: equality for black and white
// +ve: white advantage, -ve: black advantage
{
  
  // give bonus points for pawn advances
  //int pawn_advance_value = advance_value(pos.bb_white_pawns(), Color::white) 
  //                         - advance_value(pos.bb_black_pawns(), Color::black);

  return material_value(pos);// + pawn_advance_value;
}
