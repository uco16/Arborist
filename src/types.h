#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
using std::vector;
using std::string;

enum class Piece {
  empty, 
  white_pawn=1, white_knight, white_bishop, white_rook, white_queen, white_king,
  black_pawn=9, black_knight, black_bishop, black_rook, black_queen, black_king
};

enum class PieceType {
  empty, pawn, knight, bishop, rook, queen, king
};

enum class Color {white, black};
Color operator~(const Color c);

struct File {
  File(int i) :index{i} {}
  int index;
};

struct Rank {
  Rank(int i) :index{i} {}
  int index;
};

struct Square {
  Square() :file{0}, rank{0} {}
  Square(File f, Rank r) :file{f}, rank{r} {}
  File file;
  Rank rank;
};

vector<Piece> placement_to_pieces(const string& piece_placement);
char piece_to_char(const Piece pc);
string pieces_to_string(const vector<Piece>& pcs);
Color char_to_color(const string& col);
char color_to_char(const Color col);
Square algebraic_to_square(const string& alg);
string square_to_alg(const Square& sq);
int square_to_int(const Square& sq);

PieceType type_of(const Piece& pc);
Color color_of(const Piece& pc);

#endif
