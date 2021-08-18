#ifndef POSITION_H
#define POSITION_H

#include "types.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

class Position {
  public:
    Position() {set("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");}
    Position(const Position& pos) 
      :board{pos.board},		      active_color{pos.active_color}, 
       castling_rights{pos.castling_rights},  enpassant_square{pos.enpassant_square},
       has_enpassant{pos.has_enpassant},      halfmove_clock{pos.halfmove_clock}, 
       fullmoves{pos.fullmoves} {}

    void set(const string& fen);

    void move(const Move& mov) {move(mov.isq, mov.fsq);}
    void move(const Square& isq, const Square& fsq) { move(square_to_int(isq), square_to_int(fsq)); }
    void move(const string& long_move_notation);

    string fen() const;
    vector<Piece> all_pieces() const;
    vector<Move> all_moves() const;

    bool isEmpty(const Square& sq) const {
      return !(1 & (bm_all >> square_to_int(sq)));
    }

    Color active_color;

  private:
    vector<Piece> board;
    string castling_rights;
    Square enpassant_square;
    bool has_enpassant {false};
    int halfmove_clock;
    int fullmoves;

    uint_fast64_t bm_all {0};
    void update_bitmap();

    vector<Move> pawn_moves(const Square& isq) const;
    vector<Move> possible_moves(const PieceType& type, const Square& isq) const;

    void move(int i, int f);
};

#endif
