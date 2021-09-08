#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
using std::vector;
using std::string;

#include <iostream>

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
  constexpr Square(const File f, const Rank r) :file{f}, rank{r} {}
  constexpr Square(const int i) :file{i%8}, rank{7-i/8} {}
  File file;
  Rank rank;
  constexpr int toInt() const { return file.index + (7-rank.index)*8; }
};

struct Move {
  Move() {}
  constexpr Move(const Square i, const Square f, const bool e) :isq{i}, fsq{f}, isEnpassant{e} {}
  constexpr Move(const Square i, const Square f) :isq{i}, fsq{f}, isEnpassant{false} {}
  Square isq;
  Square fsq;
  bool isEnpassant;
};

struct FileDiff {
  FileDiff() {}
  constexpr FileDiff(int i) :value{i} {}
  int value;
};

struct RankDiff {
  RankDiff() {}
  constexpr RankDiff(int i) :value{i} {}
  int value;
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

constexpr File operator+(const File f, const FileDiff fd) { return {f.index + fd.value}; }
constexpr Rank operator+(const Rank r, const RankDiff rd) { return {r.index + rd.value}; }
constexpr FileDiff operator-(const File f1, const File f2) { return {f1.index - f2.index}; }
constexpr RankDiff operator-(const Rank r1, const Rank r2) { return {r1.index - r2.index}; }
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

constexpr bool isInside(const Square& sq) {
  return 0 <= sq.file.index && sq.file.index < 8 && 0 <= sq.rank.index && sq.rank.index < 8;
}

std::ostream& operator<<(std::ostream& os, const Square& sq);
std::ostream& operator<<(std::ostream& os, const Move& mov);
std::ostream& operator<<(std::ostream& os, const Color col);

constexpr PieceType type_of(const Piece& pc) {
  return static_cast<PieceType>(static_cast<int>(pc)&7);
}

constexpr Color color_of(const Piece& pc) {
  return static_cast<Color>((static_cast<int>(pc)&8)>>3);
}
constexpr Color char_to_color(const char col) {
  //return col[0]=='w' ? Color::white : Color::black;
  return static_cast<Color>(static_cast<int>(col=='b'));
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

constexpr Rank base_rank(const Color col) {
  return (col==Color::white) ? 1 : 6;
}
constexpr Rank promotion_rank(const Color col) {
  return (col==Color::white) ? 7 : 0;
}

struct Rights {
  // Castling rights: K and Q denote white King and Queen side castling, 
  // and similarly k and q for black's castling rights.
  bool K:1;
  bool Q:1;
  bool k:1;
  bool q:1;
};

Rights string_to_rights(const string& s);
string rights_to_string(const Rights& r);

#endif
