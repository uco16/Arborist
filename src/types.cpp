#include "types.h"

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

// output stream representations
std::ostream& operator<<(std::ostream& os, const Square& sq) {
  return os << char('a'+sq.file.index) << char('1'+sq.rank.index);
}
std::ostream& operator<<(std::ostream& os, const Move& mov) {
  return os << mov.isq << mov.fsq;
}
std::ostream& operator<<(std::ostream& os, const Color col) {
  return os << (col==Color::white ? "white" : "black");
}

Rights string_to_rights(const string& s) {
  Rights r = {0,0,0,0};
  for (auto c: s)
  {
    if	    (c == 'K') r.K = 1;
    else if (c == 'Q') r.Q = 1;
    else if (c == 'k') r.k = 1;
    else if (c == 'q') r.q = 1;
  }
  return r;
}

string rights_to_string(const Rights& r) {
  string s {""};
  if (r.K) s += "K";
  if (r.Q) s += "Q";
  if (r.k) s += "k";
  if (r.q) s += "q";
  return s;
}
