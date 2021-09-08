#include "position.h"
#include "misc.h"  // for split
#include <iostream>

void Position::populate_from(const string& fen_placement) {
  clear();

  int i_c {0};
  int i_sq {0};
  while (i_sq < 64) {
    char c {fen_placement[i_c]};
    i_c++;

    if (isdigit(c)) {
      for (int j=0; j<(c-'0'); j++) 
      {
	set(i_sq, Piece::empty);
	i_sq++;
      }
      continue;
    }
    else if (c=='p') set(i_sq, Piece::black_pawn); 
    else if (c=='P') set(i_sq, Piece::white_pawn);
    else if (c=='/') continue;
    else if (c=='r') set(i_sq, Piece::black_rook);
    else if (c=='b') set(i_sq, Piece::black_bishop);
    else if (c=='n') set(i_sq, Piece::black_knight);
    else if (c=='R') set(i_sq, Piece::white_rook);
    else if (c=='B') set(i_sq, Piece::white_bishop);
    else if (c=='N') set(i_sq, Piece::white_knight);
    else if (c=='q') set(i_sq, Piece::black_queen);
    else if (c=='Q') set(i_sq, Piece::white_queen);
    else if (c=='k') set(i_sq, Piece::black_king);
    else if (c=='K') set(i_sq, Piece::white_king);
    i_sq++;
  }
}

void Position::set(const string& fen)
{
  vector<string> fen_parts {split(fen)};
  populate_from(fen_parts[0]);
  active_color	    = char_to_color(fen_parts[1][0]);
  castling_rights   = string_to_rights(fen_parts[2]);
  if (fen_parts[3]!="-") {
    has_enpassant = true;
    enpassant_square = algebraic_to_square(fen_parts[3]);
  }
  halfmove_clock    = fen_parts[4][0];
  fullmoves	    = fen_parts[5][0];
}

string Position::fen() const
{
  vector<Piece> board_vector (board, board+64);
  string piece_placement {pieces_to_string(board_vector)};
  char color {color_to_char(active_color)};
  string rights {rights_to_string(castling_rights)};
  string enpassant = has_enpassant ? square_to_alg(enpassant_square) : "-";
  
  return piece_placement + " " 
    + color + " " 
    + rights + " " 
    + enpassant + " " 
    + (char)halfmove_clock + " " 
    + (char)fullmoves;
}

void Position::move(const Square& isq, const Square& fsq)
{
  const Piece pc {get(isq)};
  set(fsq, pc);
  clear(isq);
  active_color = ~active_color;

  // handle enpassant capture
  if (type_of(pc)==PieceType::pawn && fsq==enpassant_square)
    clear({fsq.file, isq.rank});

  update_enpassant_square(isq.toInt(), fsq.toInt());
}

void Position::update_enpassant_square(const int i, const int f)
{
  if (type_of(board[f])==PieceType::pawn && 
      abs(Square(f).rank.index-Square(i).rank.index)==2 )
  {
    enpassant_square = Square(Square(f).file, Square(i).rank);
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

// function to undo a move
void Position::unmove(const Move& mov, const Piece captured_piece, const Square& previous_enpassant_sq) {
  const Piece moved_pc {get(mov.fsq)};
  set(mov.isq, moved_pc);
  set(mov.fsq, captured_piece);
  active_color = ~active_color;

  if (mov.isEnpassant)
  {
    if (active_color==Color::white)
      set(mov.fsq + Direction(0, -1), Piece::black_pawn);
    else
      set(mov.fsq + Direction(0, 1),  Piece::white_pawn);
  }

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

//vector<Move> Position::pawn_moves(const Square& isq) const
//{
//  vector<Move> moves;
//  const Color pawn_color {color_of(board[isq.toInt()])};
//  const int factor = color_factor(pawn_color); // determines movement direction
//
//  // one step is always a valid move pattern since there is never a pawn on the lowest rank
//  const Square fsq = isq + (Direction){0, factor}; 
//  if (isEmpty(fsq))
//    moves.push_back({isq, fsq});
//
//  // two steps are only possible from base ranks
//  if (isq.rank==base_rank(pawn_color))
//  {
//    const Square fsq = isq + (Direction){0, 2*factor}; 
//    if (isEmpty(fsq) && isEmpty(isq+(Direction){0, 1*factor}))
//      moves.push_back({isq, fsq});
//  }
//
//  // diagonal captures
//  vector<Square> capture_squares;
//  if (isq.file.index > 0)
//    capture_squares.push_back(isq + (Direction){-1, factor});
//  if (isq.file.index < 7)
//    capture_squares.push_back(isq + (Direction){ 1, factor});
//  for (Square fsq: capture_squares)
//  {
//    const Piece pc = board[fsq.toInt()];
//    if (fsq==enpassant_square || (type_of(pc)!=PieceType::empty && color_of(pc)!=pawn_color))
//      moves.push_back({isq, fsq});
//  }
//  return moves;
//}

const vector<Direction> knight_directions = {
  { 1,  2}, { 2,  1}, {-1,  2}, {-2,  1}, 
  {-2, -1}, { 2, -1}, { 1, -2}, {-1, -2}
};

const vector<Direction> king_directions = {
  {0,  1}, { 1,  1}, { 1, 0}, { 1, -1},
  {0, -1}, {-1, -1}, {-1, 0}, {-1,  1}
};

vector<Move> dir_moves(const Square& isq, const vector<Direction>& directions, const Bitboard& blocked) {
  vector<Move> moves;
  for (Direction dir: directions)
  {
    const Square fsq {isq+dir};
    if (isInside(fsq) && !getBit(blocked, 63-fsq.toInt()))
      moves.push_back({isq, fsq});
  }
  return moves;
}

vector<Square> bb_to_squares(const Bitboard& bb) {
  vector<Square> squares;
  for (int i=0; i<64; i++)
  {
    if (getBit(bb, i))
      squares.push_back(Square(63-i));
  }
  return squares;
}
vector<Move> bb_to_moves(const Square& isq, const Bitboard& fsq_bb) {
  vector<Move> moves;
  for (Square fsq: bb_to_squares(fsq_bb))
    moves.push_back({isq, fsq});
  return moves;
}
vector<Move> rook_moves(const Square& isq, const Bitboard& foes, const Bitboard& friends) {
  const Bitboard occupancy {foes & friends};
  return bb_to_moves(isq, rook_bb(isq, occupancy)&~friends);
}
vector<Move> bishop_moves(const Square& isq, const Bitboard& foes, const Bitboard& friends) {
  const Bitboard occupancy {foes & friends};
  return bb_to_moves(isq, bishop_bb(isq, occupancy)&~friends);
}
vector<Move> queen_moves(const Square& isq, const Bitboard& foes, const Bitboard& friends) {
  const Bitboard occupancy {foes & friends};
  return bb_to_moves(isq, queen_bb(isq, occupancy)&~friends);
}

vector<Move> pawn_moves(const Square& isq, const Color col, const Bitboard& foes,
			const Bitboard& friends, const Bitboard& enpassant_sq_bb) {
  return bb_to_moves(isq, pawn_bb(isq, col, friends, foes, enpassant_sq_bb));
}

// TODO:
//Bitboard Position::move_bb(const Square& isq)
//
//then 
//possible_moves(isq) = bb_to_moves(isq, move_bb(isq))

vector<Move> Position::possible_moves(const PieceType& type, const Square& isq) const
{
  const Bitboard& friends {bb_active_color()^square_to_bb(isq)};
  const Bitboard& foes {bb_all()^bb_active_color()};
  const Bitboard& enpassant_sq_bb {has_enpassant ? square_to_bb(enpassant_square) : OBB};

  switch(type) {
    case PieceType::pawn  : return pawn_moves (isq, active_color, foes, friends, enpassant_sq_bb);
    case PieceType::knight: return dir_moves  (isq, knight_directions,	friends);
    case PieceType::king  : return dir_moves  (isq, king_directions,	friends);
    case PieceType::rook  : return rook_moves (isq, foes, friends);
    case PieceType::bishop: return bishop_moves(isq, foes, friends );
    case PieceType::queen : return queen_moves(isq, foes, friends );
  }
  return {};
}

vector<Move> Position::all_moves() const
// all moves that are possible for the currently active color
{
  vector<Move> moves;
  for (int i=0; i<64; i++)
  {
    const Square isq {i};
    if (isOwnColor(isq))
    {
      const Piece pc = board[i];
      vector<Move> poss_moves {possible_moves(type_of(pc), isq)};
      moves.insert(moves.end(), poss_moves.begin(), poss_moves.end());
    }
  }
  return moves;
}
