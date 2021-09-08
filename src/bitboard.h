#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"
#include <iostream>

using Bitboard = uint_fast64_t;

constexpr Bitboard OBB {0};
constexpr Bitboard IBB {1};
constexpr Bitboard LBB {IBB << 63};
constexpr Bitboard ALL_BB {~OBB};
constexpr Bitboard FIRST_FILE_BB {0x8080808080808080};
constexpr Bitboard FIRST_RANK_BB {0x00000000000000FF};
constexpr Bitboard MAIN_DIAG_BB  {0x8040201008040201};
constexpr Bitboard OFF_DIAG_BB   {0x0102040810204080};

inline constexpr Bitboard int_to_bitboard(const int i) {
  return IBB << i;
}

inline constexpr Bitboard square_to_bb(const Square& sq) {
  return IBB << (63-sq.toInt());
}

inline constexpr bool getBit(const Bitboard& b, const int i) {
  return (b >> i) & 1;
}

inline constexpr Bitboard& setBit(Bitboard& b, const int i) {
  return b |= (IBB << i);
}

inline constexpr Bitboard& unsetBit(Bitboard& b, const int i) {
  return b &= ~(IBB << i);
}

inline constexpr Bitboard& toggleBit(Bitboard& b, const int i) {
  return b ^= (IBB << i);
}

inline constexpr int bitcount(const Bitboard& bb) {
  return __builtin_popcountll(bb);
}

inline constexpr Bitboard file_bb(const File f) {
  return FIRST_FILE_BB >> f.index;
}

inline constexpr Bitboard rank_bb(const Rank r) {
  return FIRST_RANK_BB << 8*r.index;
}

inline constexpr Bitboard plus_bb(const File f, const Rank r) {
  return file_bb(f) | rank_bb(r);
}

inline constexpr Bitboard plus_bb(const Square& sq) {
  return plus_bb(sq.file, sq.rank);
}

// Bit-shifting diagonals is (almost) straightforward:
//
//  Main diag:
//
//   0010      0001      0000      1000      0100      0010      0001
//   0001  <<  0000  <<  1000  <<  0100  >>  0010  >>  0001  >>  0000
//   0000      1000      0100      0010      0001      0000      1000
//   1000      0100      0010      0001      0000      1000      0100
//
// When shifting to the right n times, we need to discard the bottom n rows.
// When shifting to the left n times, we need to discard the top n rows.
// We can do this by masking or by shifting off the edges.
//
//
// note that the unique ones are only the five in the center (although we do not use this observation)
//
//             0001      0000      1000      0100      0010
//             0000  <<  1000  <<  0100  >>  0010  >>  0001
//             1000      0100      0010      0001      0000
//             0100      0010      0001      0000      1000
//
//  Off-diag:
//
//   1001      0100      0010      0001      0000      0000      0000
//   0010  <<  1001  <<  0100  <<  0010  >>  1001  >>  0100  >>  0010
//   0100      0010      1001      0100      0010      1001      0100
//   0000      0000      0000      1000      0100      0010      1001
//
//  Here it is clear that the shift approach is easier.

inline constexpr Bitboard main_diag_bb(const int shift_count) {
  return (shift_count >= 0) ? (MAIN_DIAG_BB >> 9*shift_count) << 8*shift_count
			    : (MAIN_DIAG_BB << -9*shift_count)>> -8*shift_count;
}

inline constexpr Bitboard off_diag_bb(const int shift_count) {
  return (shift_count >= 0) ? (OFF_DIAG_BB >> 8*shift_count)
			    : (OFF_DIAG_BB >> -8*shift_count) << -9*shift_count;
}

inline constexpr Bitboard times_bb(const Square& sq) {
  int main_shift = sq.file.index-(7-sq.rank.index);
  int off_shift  = sq.file.index-sq.rank.index;
  return main_diag_bb(main_shift) | off_diag_bb(off_shift);
}

//  TODO: Create bit maps for king, knight, and maybe also pawns
//  Knight pattern:    King pattern:
//
//     00000000          00000000
//     00101000          00000000
//     01000100          00111000
//     00020000          00121000
//     01000100          00111000
//     00101000          00000000
//     00000000          00000000
//     00000000          00000000
//

// --- SLIDING PIECE OBSTRUCTION DETECTION ---

// large masks
constexpr Bitboard fill_to(const int i) {
  return ALL_BB << (63-i);
}
constexpr Bitboard fill_from(const int i) {
  return ALL_BB >> i;
}
constexpr Bitboard fill_before(const int i) {
  return fill_to(i) << 1;
}
constexpr Bitboard fill_after(const int i) {
  return fill_from(i) >> 1;
}

// rays
constexpr Bitboard S_ray(const Square& sq) {
  return file_bb(sq.file) & fill_after(sq.toInt());
}
constexpr Bitboard N_ray(const Square& sq) {
  return file_bb(sq.file) & fill_before(sq.toInt());
}
constexpr Bitboard E_ray(const Square& sq) {
  return rank_bb(sq.rank) & fill_after(sq.toInt());
}
constexpr Bitboard W_ray(const Square& sq) {
  return rank_bb(sq.rank) & fill_before(sq.toInt());
}
constexpr Bitboard NE_ray(const Square& sq) {
  int off_shift  = sq.file.index-sq.rank.index;
  return off_diag_bb(off_shift) & fill_before(sq.toInt());
}
constexpr Bitboard SW_ray(const Square& sq) {
  int off_shift  = sq.file.index-sq.rank.index;
  return off_diag_bb(off_shift) & fill_after(sq.toInt());
}
constexpr Bitboard NW_ray(const Square& sq) {
  int main_shift = sq.file.index-(7-sq.rank.index);
  return main_diag_bb(main_shift) & fill_before(sq.toInt());
}
constexpr Bitboard SE_ray(const Square& sq) {
  int main_shift = sq.file.index-(7-sq.rank.index);
  return main_diag_bb(main_shift) & fill_after(sq.toInt());
}

// truncated rays
constexpr Bitboard S_tray(const Square& sq, const Bitboard& obstructions) {
  return S_ray(sq)^S_ray(__builtin_clzll(obstructions & S_ray(sq)));
}
constexpr Bitboard E_tray(const Square& sq, const Bitboard& obstructions) {
  return E_ray(sq)^E_ray(__builtin_clzll(obstructions & E_ray(sq)));
}
constexpr Bitboard SE_tray(const Square& sq, const Bitboard& obstructions) {
  return SE_ray(sq)^SE_ray(__builtin_clzll(obstructions & SE_ray(sq)));
}
constexpr Bitboard SW_tray(const Square& sq, const Bitboard& obstructions) {
  return SW_ray(sq)^SW_ray(__builtin_clzll(obstructions & SW_ray(sq)));
}
constexpr Bitboard N_tray(const Square& sq, const Bitboard& obstructions) {
  return N_ray(sq)^N_ray(__builtin_ctzll(obstructions & N_ray(sq)));
}
constexpr Bitboard W_tray(const Square& sq, const Bitboard& obstructions) {
  return W_ray(sq)^W_ray(__builtin_ctzll(obstructions & W_ray(sq)));
}
constexpr Bitboard NE_tray(const Square& sq, const Bitboard& obstructions) {
  return NE_ray(sq)^NE_ray(__builtin_ctzll(obstructions & NE_ray(sq)));
}
constexpr Bitboard NW_tray(const Square& sq, const Bitboard& obstructions) {
  return NW_ray(sq)^NW_ray(__builtin_ctzll(obstructions & NW_ray(sq)));
}

// truncated sliding piece patterns
constexpr Bitboard rook_bb(const Square& sq, const Bitboard& obs) {
  return S_tray(sq, obs) & E_tray(sq, obs) & N_tray(sq, obs) & W_tray(sq, obs);
}
constexpr Bitboard bishop_bb(const Square& sq, const Bitboard& obs) {
  return NE_tray(sq, obs) & SE_tray(sq, obs) & NW_tray(sq, obs) & SW_tray(sq, obs);
}
constexpr Bitboard queen_bb(const Square& sq, const Bitboard& obs) {
  return rook_bb(sq, obs) | bishop_bb(sq, obs);
}

// --- pawn patterns ---

constexpr Bitboard shift_ranks(const Bitboard& bb, const int shift_count) {
  return shift_count >= 0 ? bb << 8*shift_count : bb >> 8*shift_count;
}

// when shifting right and left, we first need to clear the digits that will be shifted off
constexpr Bitboard shift_right(const Bitboard& bb) {
  return (bb & ~file_bb(7)) >> 1;
}
constexpr Bitboard shift_left(const Bitboard& bb) {
  return (bb & ~file_bb(0)) << 1;
}

constexpr Bitboard pawn_attack_bb(const Bitboard& pawns, const Color col) {
  // represents all the squares that the pawns reach via an attack
  return shift_ranks(shift_left(pawns)&shift_right(pawns), color_factor(col));
}

constexpr Bitboard pawn_bb(const Square& sq, const Color col,
		       const Bitboard& friends, const Bitboard& foes,
		       const Bitboard& enpassant_sq_bb=OBB) {
  const Bitboard occupied {friends&foes};
  const Bitboard sq_bb {square_to_bb(sq)};
  const Bitboard single_step { shift_ranks(sq_bb, color_factor(col)) & ~occupied};
  const Bitboard double_step { 
    shift_ranks(
      shift_ranks(
	  sq_bb & rank_bb(base_rank(col))  // if we have started from the base rank
	  , color_factor(col)) & ~occupied  // ...and the square one above is empty
      , color_factor(col)) & ~occupied  // ...and the square two above is empty
  };
  const Bitboard captures { pawn_attack_bb(sq_bb, col)&(foes|enpassant_sq_bb) };
  return single_step | double_step | captures;
}

#endif
