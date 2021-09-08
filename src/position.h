#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "bitboard.h"
#include "misc.h"  // for display

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <iostream>
#include <algorithm> // for std::fill

class Position {
  public:
    Position() { set("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); }
    Position(const string& fen) { set(fen);}

    void move(const Square& isq, const Square& fsq);
    void move(const Move& mov) {move(mov.isq, mov.fsq);}
    void move(const string& long_move_notation);

    void unmove(const Move& mov, const Piece captured_piece, const Square& previous_enpassant_sq);

    string fen() const;
    vector<Piece> all_pieces() const;
    vector<Move> all_moves() const;

    bool isEmpty(const Square& sq) const { return !getBit(_all, 63-sq.toInt()); }
    bool isWhite(const Square& sq) const { return getBit(_white, 63-sq.toInt()); }
    bool isBlack(const Square& sq) const { return getBit(bb_black(), 63-sq.toInt()); }
    bool isOwnColor(const Square& sq) const{ return getBit(bb_active_color(), 63-sq.toInt()); }
    PieceType type_at(const Square& sq) const { return type_of(board[sq.toInt()]); }

    Color active_color;

    Bitboard bb_all() const { return _all; }
    Bitboard bb_white() const { return _white; }
    Bitboard bb_black() const { return _all ^ _white; }
    Bitboard bb_active_color() const { return active_color==Color::white ? _white : bb_black(); }
    Bitboard bb_pawns() const { return _pawns; }
    Bitboard bb_white_pawns() const { return _pawns & _white; }
    Bitboard bb_black_pawns() const { return _pawns & bb_black(); }
    Bitboard bb_kings() const	{ return _kings; }
    Bitboard bb_knights() const { return _knights; }
    Bitboard bb_queens() const	{ return _plus_sliders & _times_sliders; }
    Bitboard bb_rooks() const	{ return _plus_sliders & ~_times_sliders; }
    Bitboard bb_bishops() const { return _times_sliders & ~_plus_sliders; }

    Piece get(const Square& sq) const { return board[sq.toInt()]; }

    void clear() {
      std::fill(board, board+64, Piece::empty);
      _all = OBB;
      _white = OBB;
      _pawns = OBB;
      _kings = OBB;
      _knights = OBB;
      _plus_sliders = OBB;
      _times_sliders = OBB;
    }
    void clear(const Square& sq) { clear(sq.toInt()); }
    void clear(const int i) {
      board[i] = Piece::empty; 
      //std::cout << "unsetting bit " << 63-i << "\n";
      unsetBit(_all, 63-i);
      unsetBit(_white, 63-i);
      unsetBit(_pawns, 63-i);
      unsetBit(_kings, 63-i);
      unsetBit(_knights, 63-i);
      unsetBit(_plus_sliders,  63-i);
      unsetBit(_times_sliders, 63-i);
    }

    void set(const string& fen);
    void set(const Square& sq, const Piece pc) { set(sq.toInt(), pc); }
    void set(const int i, const Piece pc) {
      clear(i);
      board[i] = pc;

      if (pc==Piece::empty)
	return;

      //std::cout << "setting bit " << 63-i << "\n";
      setBit(_all, 63-i);
      if      (color_of(pc)==Color::white)	  setBit(_white, 63-i);
      if      (type_of(pc) == PieceType::pawn)	  setBit(_pawns, 63-i);
      else if (type_of(pc) == PieceType::king)	  setBit(_kings, 63-i);
      else if (type_of(pc) == PieceType::knight)  setBit(_knights, 63-i);
      else if (type_of(pc) == PieceType::rook)	  setBit(_plus_sliders, 63-i);
      else if (type_of(pc) == PieceType::bishop)  setBit(_times_sliders, 63-i);
      else if (type_of(pc) == PieceType::queen) { setBit(_times_sliders, 63-i);
						  setBit(_plus_sliders,  63-i); }
    }

    void print_bitboards() const {
      std::cout << "all: ";
      display(std::cout, _all);
      std::cout << "white: ";
      display(std::cout, _white);
      std::cout << "pawns: ";
      display(std::cout, _pawns);
      std::cout << "kings: ";
      display(std::cout, _kings);
      std::cout << "knights: ";
      display(std::cout, _knights);
      std::cout << "plus_sliders: ";
      display(std::cout, _plus_sliders);
      std::cout << "times_sliders: ";
      display(std::cout, _times_sliders);
    }

    Square enpassant() const { return enpassant_square; }

  private:
    // fen data
    Piece board [64];
    Rights castling_rights;
    Square enpassant_square;
    bool has_enpassant {false};
    int halfmove_clock;
    int fullmoves;

    void populate_from(const string& fen_placement);

    //vector<Move> pawn_moves(const Square& isq) const;
    vector<Move> possible_moves(const PieceType& type, const Square& isq) const;
    void update_enpassant_square(const int i, const int f);

    // internal Bitboards
    Bitboard _all;
    Bitboard _white;
    Bitboard _pawns;
    Bitboard _kings;
    Bitboard _knights;
    Bitboard _plus_sliders;
    Bitboard _times_sliders;
};

#endif
