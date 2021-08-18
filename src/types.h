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

struct File {
  File () {}
  constexpr File(int i) :index{i} {}
  int index;
};

struct Rank {
  Rank () {}
  constexpr Rank(int i) :index{i} {}
  int index;
};

struct Square {
  Square() {}
  constexpr Square(File f, Rank r) :file{f}, rank{r} {}
  File file;
  Rank rank;
};

struct Move {
  Move() {}
  constexpr Move(Square i, Square f) :isq{i}, fsq{f} {}
  Square isq;
  Square fsq;
};

struct FileDiff {
  FileDiff() {}
  constexpr FileDiff(int i) :index{i} {}
  int index;
};

struct RankDiff {
  RankDiff() {}
  constexpr RankDiff(int i) :index{i} {}
  int index;
};

struct Direction {
  Direction() {}
  constexpr Direction(FileDiff f, RankDiff r): fd{f}, rd{r} {}
  FileDiff fd;
  RankDiff rd;
};

constexpr Color operator~(const Color c) {
  return static_cast<Color>(static_cast<int>(c)^1);
}

constexpr File operator+(const File f, const FileDiff fd) {return {f.index + fd.index};}
constexpr Rank operator+(const Rank r, const RankDiff rd) {return {r.index + rd.index};}
constexpr Square operator+(const Square& sq, const Direction& d) { return {sq.file + d.fd, sq.rank + d.rd}; }

constexpr bool operator==(const Rank a, const Rank b) {return a.index==b.index;}
constexpr bool operator==(const File a, const File b) {return a.index==b.index;}
constexpr bool operator==(const Square& a, const Square& b) { return a.rank==b.rank && a.file==b.file; }

string pieces_to_string(const vector<Piece>& pcs);
string square_to_alg(const Square& sq);
Square algebraic_to_square(const string& alg);

constexpr char piece_to_char(const Piece pc) {
  return " PNBRQK  pnbrqk"[static_cast<int>(pc)];
}

constexpr Square int_to_square(int i) {
  return Square(i%8, 7-i/8);
}

string move_to_string(const Move& mov);

constexpr PieceType type_of(const Piece& pc) {
  return static_cast<PieceType>(static_cast<int>(pc)&7);
}

constexpr Color color_of(const Piece& pc) {
  return static_cast<Color>((static_cast<int>(pc)&8)>>3);
}
constexpr Color char_to_color(const char col) {
  //return col[0]=='w' ? Color::white : Color::black;
  return static_cast<Color>(static_cast<int>(col=='w'));
}

constexpr int square_to_int(const Square& sq) {
  return sq.file.index + (7-sq.rank.index)*8;
}

constexpr int color_factor(const Color col) {
  return 1-2*static_cast<int>(col);
}

constexpr char color_to_char(const Color col) {
  //return col==Color::white ? 'w' : 'b';
  return 'w'^(21*static_cast<int>(col));
}


#endif
