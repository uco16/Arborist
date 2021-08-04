#include "types.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

Color operator~(const Color c) {
  return c==Color::white ? Color::black : Color::white;
}

vector<Piece> placement_to_pieces(const string& piece_placement)
{
  vector<Piece> pcs;
  for (char c: piece_placement) {
    if (isdigit(c)) {
      for (int i=0; i<(c-'0'); i++)
	pcs.push_back(Piece::empty);
    }
    else if (c=='p') pcs.push_back(Piece::black_pawn);
    else if (c=='n') pcs.push_back(Piece::black_knight);
    else if (c=='b') pcs.push_back(Piece::black_bishop);
    else if (c=='r') pcs.push_back(Piece::black_rook);
    else if (c=='q') pcs.push_back(Piece::black_queen);
    else if (c=='k') pcs.push_back(Piece::black_king);
    else if (c=='P') pcs.push_back(Piece::white_pawn);
    else if (c=='N') pcs.push_back(Piece::white_knight);
    else if (c=='B') pcs.push_back(Piece::white_bishop);
    else if (c=='R') pcs.push_back(Piece::white_rook);
    else if (c=='Q') pcs.push_back(Piece::white_queen);
    else if (c=='K') pcs.push_back(Piece::white_king);
  }
  return pcs;
}

char piece_to_char(const Piece pc)
{
  if (pc==Piece::empty)		return ' ';
  if (pc==Piece::white_pawn)	return 'P';
  if (pc==Piece::white_knight)	return 'N';
  if (pc==Piece::white_bishop)	return 'B';
  if (pc==Piece::white_rook)	return 'R';
  if (pc==Piece::white_queen)	return 'Q';
  if (pc==Piece::white_king)	return 'K';
  if (pc==Piece::black_pawn)	return 'p';
  if (pc==Piece::black_knight)	return 'n';
  if (pc==Piece::black_bishop)	return 'b';
  if (pc==Piece::black_rook)	return 'r';
  if (pc==Piece::black_queen)	return 'q';
  if (pc==Piece::black_king)	return 'k';
  else return 'x';  // to prevent compiler warning
}

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

Color char_to_color(const string& col)
{
  return col[0]=='w' ? Color::white : Color::black;
}

char color_to_char(const Color col)
{
  return col==Color::white ? 'w' : 'b';
}

Square algebraic_to_square(const string& alg)
{
  File file {alg[0]-'a'};
  Rank rank {alg[1]-'1'};
  Square sq {file, rank};
  return sq;
}

string square_to_alg(const Square& sq)
{
  char letter = 'a'+sq.file.index;
  char num = '1'+sq.rank.index;
  string ans;
  ans += letter;
  ans += num;
  return ans;
}

int square_to_int(const Square& sq)
{
  int col = sq.file.index;
  int row = 7-(sq.rank.index);
  return col + row*8;
}

PieceType type_of(const Piece& pc)
{
  return static_cast<PieceType>(static_cast<int>(pc)&7);
}

Color color_of(const Piece& pc)
{
  return static_cast<Color>((static_cast<int>(pc)&8)>>3);
}
