#include "types.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

string pieces_to_string(const vector<Piece>& pcs)
{
  string piece_placement;
  int cnt {-1};
  int empties {0};
  for (auto pc: pcs) {
    cnt++;
    if (cnt == 8) {
      if (empties > 0) {
	piece_placement += (char)('0'+empties);
	empties = 0;
      }
      piece_placement += "/";
      cnt = 0;
    }
    if (pc==Piece::empty)
      empties++;
    else {
      if (empties > 0) {
	piece_placement += (char)('0'+empties);
	empties = 0;
      }
      piece_placement += piece_to_char(pc);
    }
  }
  return piece_placement;
}

Square algebraic_to_square(const string& alg) { 
  return Square(File(alg[0]-'a'), Rank(alg[1]-'1'));
}

string square_to_alg(const Square& sq) {
  return {char('a'+sq.file.index), char('1'+sq.rank.index)};
}

string move_to_string(const Move& mov) {
  return square_to_alg(mov.isq) + square_to_alg(mov.fsq);
}
