#ifndef POSITION_H
#define POSITION_H

#include "types.h"

#include <string>
using std::string;
#include <vector>
using std::vector;

class Position {
  public:
    Position() {}
    void set(const string& fen);
    void move(int i, int f);
    void move(const Square& isq, const Square& fsq);
    void move(const string& long_move_notation);
    string fen() const;
    vector<Piece> all_pieces() const;

  private:
    vector<Piece> board;
    Color active_color;
    string castling_rights;
    Square enpassant_square;
    bool has_enpassant {false};
    int halfmove_clock;
    int fullmoves;
};

#endif
