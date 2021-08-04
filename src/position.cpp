#include "position.h"
#include "misc.h"  // for split
#include <iostream>

void Position::set(const string& fen)
{
  vector<string> fen_parts {split(fen)};
  board		    = placement_to_pieces(fen_parts[0]);
  active_color	    = char_to_color(fen_parts[1]);
  castling_rights   = fen_parts[2];
  if (fen_parts[3]!="-") {
    enpassant_square = algebraic_to_square(fen_parts[3]);
    has_enpassant = true;
  }
  halfmove_clock    = fen_parts[4][0];
  fullmoves	    = fen_parts[5][0];
}

string Position::fen() const
{
  string piece_placement {pieces_to_string(board)};
  char color {color_to_char(active_color)};
  string enpassant {"-"};
  if (has_enpassant)
    enpassant = square_to_alg(enpassant_square);
  string fen {piece_placement + " " + color + " " + castling_rights + " " + enpassant + " " + (char)halfmove_clock + " " + (char)fullmoves};
  return fen;
}

void Position::move(int i, int f)
{
  board[f] = board[i]; 
  board[i] = Piece::empty;
  active_color = ~active_color;
}

void Position::move(const Square& isq, const Square& fsq)
{
 int i = square_to_int(isq);
 int f = square_to_int(fsq);
 move(i, f);
}

void Position::move(const string& mv)
{
  Square isq {algebraic_to_square(mv.substr(0, 2))};
  Square fsq {algebraic_to_square(mv.substr(2, 2))};
  move(isq, fsq);
}

vector<Piece> Position::all_pieces() const
{
  vector<Piece> pcs;
  for (Piece pc: board)
  {
    if (pc != Piece::empty)
      pcs.push_back(pc);
  }
  return pcs;
}
