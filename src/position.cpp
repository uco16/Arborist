#include "position.h"
#include "misc.h"  // for split
#include <iostream>

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

void Position::set(const string& fen)
{
  vector<string> fen_parts {split(fen)};
  board		    = placement_to_pieces(fen_parts[0]);
  update_bitmap();
  active_color	    = char_to_color(fen_parts[1][0]);
  castling_rights   = fen_parts[2];
  if (fen_parts[3]!="-") {
    enpassant_square = algebraic_to_square(fen_parts[3]);
    has_enpassant = true;
  }
  halfmove_clock    = fen_parts[4][0];
  fullmoves	    = fen_parts[5][0];
}

void Position::update_bitmap()
{
  for (int i=0; i<64; i++)
  {
    if (type_of(board[i])!=PieceType::empty)
    {
      uint_fast64_t mask {1};
      bm_all |= (mask << i);
    }
  }
}

string Position::fen() const
{
  string piece_placement {pieces_to_string(board)};
  char color {color_to_char(active_color)};
  string enpassant = has_enpassant ? square_to_alg(enpassant_square) : "-";

  return piece_placement + " " 
    + color + " " 
    + castling_rights + " " 
    + enpassant + " " 
    + (char)halfmove_clock + " " 
    + (char)fullmoves;
}

void Position::move(int i, int f)
{
  board[f] = board[i]; 
  board[i] = Piece::empty;
  active_color = ~active_color;
  
  // update bitmap
  uint_fast64_t mask {1};
  bm_all &= ~(mask << i);
  bm_all |= (mask << f);

  // handle enpassant capture
  if (type_of(board[f])==PieceType::pawn && int_to_square(f)==enpassant_square)
  {
    Square capture_square {int_to_square(f).file, int_to_square(i).rank};
    int c {square_to_int(capture_square)};
    board[c] = Piece::empty;
    bm_all &= ~(mask << c);
  }

  // keep track of en passant square
  if (type_of(board[f])==PieceType::pawn && 
      abs(int_to_square(f).rank.index-int_to_square(i).rank.index)==2 )
  {
    enpassant_square = Square(int_to_square(f).file, int_to_square(i).rank);
    has_enpassant = true;
  }
  else
    has_enpassant = false;

}

void Position::move(const string& mv) {
  Square isq {algebraic_to_square(mv.substr(0, 2))};
  Square fsq {algebraic_to_square(mv.substr(2, 2))};
  move({isq, fsq});
}

vector<Piece> Position::all_pieces() const
{
  vector<Piece> pcs;
  for (Piece pc: board) {
    if (pc != Piece::empty)
      pcs.push_back(pc);
  }
  return pcs;
}

vector<Move> Position::pawn_moves(const Square& isq) const
{
  vector<Move> moves;
  Color pawn_color = color_of(board[square_to_int(isq)]);

  // one step is always a valid move pattern since there is never a pawn on the lowest rank
  int factor = color_factor(pawn_color);
  Direction dir {0, factor};
  Square fsq = isq + dir; 
  if (isEmpty(fsq))
    moves.push_back({isq, fsq});

  // two steps are only possible from base ranks
  if ((isq.rank.index==1 && pawn_color==Color::white)
      || (isq.rank.index==6 && pawn_color==Color::black))
  {
    Square fsq = isq + (Direction){0, 2*factor}; 
    if (isEmpty(fsq) && isEmpty(isq+(Direction){0, 1*factor}))
      moves.push_back({isq, fsq});
  }

  // diagonal captures
  if (isq.file.index > 0)
  {
    Direction dir {-1, factor};
    Square fsq = isq + dir;
    Piece pc = board[square_to_int(fsq)];
    if (fsq==enpassant_square || (type_of(pc)!=PieceType::empty && color_of(pc)==~pawn_color))
      moves.push_back({isq, fsq});
  }
  if (isq.file.index < 7)
  {
    Direction dir {1, factor};
    Square fsq = isq + dir;
    Piece pc = board[square_to_int(fsq)];
    if (fsq==enpassant_square || (type_of(pc)!=PieceType::empty && color_of(pc)==~pawn_color))
      moves.push_back({isq, fsq});
  }
  return moves;
}

const vector<Direction> knight_directions = {
  { 1,  2}, {-1,  2}, {-2,  1}, { 2,  1},
  {-2, -1}, { 2, -1}, { 1, -2}, {-1, -2}
};

const vector<Direction> king_directions = {
  {0,  1}, { 1,  1}, { 1, 0}, { 1, -1},
  {0, -1}, {-1, -1}, {-1, 0}, {-1,  1}
};

vector<Square> dir_squares(const Square& isq, const vector<Direction>& directions) {
  vector<Square> squares;
  for (auto dir: directions) 
    squares.push_back(isq+dir);
  return squares;
}

bool isInside(const Square& sq) {
  return 0 <= sq.file.index && sq.file.index < 8 && 0 <= sq.rank.index && sq.rank.index < 8;
}

vector<Move> dir_moves(const Square& isq, const vector<Direction>& directions) {
  vector<Move> moves;
  for (auto dir: directions)
  {
    if (isInside(isq+dir))
      moves.push_back({isq, isq+dir});
  }
  return moves;
}

vector<Move> Position::possible_moves(const PieceType& type, const Square& isq) const
{
  if	  (type==PieceType::pawn)   return pawn_moves(isq);
  else if (type==PieceType::knight) return dir_moves(isq, knight_directions);
  else if (type==PieceType::king)   return dir_moves(isq, king_directions);
  else return {};
}

vector<Move> Position::all_moves() const
// all moves that are possible for the currently active color
{
  vector<Move> moves;
  for (int i=0; i<64; i++)
  {
    Piece pc = board[i];
    if (pc!=Piece::empty && color_of(pc)==active_color)
    {
      Square isq {int_to_square(i)};
      vector<Move> poss_moves {possible_moves(type_of(pc), isq)};
      moves.insert(moves.end(), poss_moves.begin(), poss_moves.end());
    }
  }
  return moves;
}
