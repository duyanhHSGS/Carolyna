#pragma once

#include "Types.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#if defined(__GNUC__) || defined(__clang__)
#define HAS_BUILTIN_CTZLL
#define HAS_BUILTIN_CLZLL
#define HAS_BUILTIN_POPCOUNTLL
#elif defined(_MSC_VER)
#include <intrin.h>
#define HAS_MSVC_INTRINSICS
#endif

struct Move;

struct ChessBitboardUtils
{
	// ========================================================================
	// Compile-time Constants
	// ========================================================================
	inline static constexpr uint64_t RANK_1 = 0x00000000000000FFULL;
	inline static constexpr uint64_t RANK_2 = 0x000000000000FF00ULL;
	inline static constexpr uint64_t RANK_3 = 0x0000000000FF0000ULL;
	inline static constexpr uint64_t RANK_4 = 0x00000000FF000000ULL;
	inline static constexpr uint64_t RANK_5 = 0x000000FF00000000ULL;
	inline static constexpr uint64_t RANK_6 = 0x0000FF0000000000ULL;
	inline static constexpr uint64_t RANK_7 = 0x00FF000000000000ULL;
	inline static constexpr uint64_t RANK_8 = 0xFF00000000000000ULL;

	inline static constexpr uint64_t FILE_A = 0x0101010101010101ULL;
	inline static constexpr uint64_t FILE_B = FILE_A << 1;
	inline static constexpr uint64_t FILE_C = FILE_A << 2;
	inline static constexpr uint64_t FILE_D = FILE_A << 3;
	inline static constexpr uint64_t FILE_E = FILE_A << 4;
	inline static constexpr uint64_t FILE_F = FILE_A << 5;
	inline static constexpr uint64_t FILE_G = FILE_A << 6;
	inline static constexpr uint64_t FILE_H = FILE_A << 7;

	// Square bitboards (brrr)
	inline static constexpr uint64_t A1_SQ_BB = 1ULL << 0;
	inline static constexpr uint64_t B1_SQ_BB = 1ULL << 1;
	inline static constexpr uint64_t C1_SQ_BB = 1ULL << 2;
	inline static constexpr uint64_t D1_SQ_BB = 1ULL << 3;
	inline static constexpr uint64_t E1_SQ_BB = 1ULL << 4;
	inline static constexpr uint64_t F1_SQ_BB = 1ULL << 5;
	inline static constexpr uint64_t G1_SQ_BB = 1ULL << 6;
	inline static constexpr uint64_t H1_SQ_BB = 1ULL << 7;

	inline static constexpr uint64_t A8_SQ_BB = 1ULL << 56;
	inline static constexpr uint64_t B8_SQ_BB = 1ULL << 57;
	inline static constexpr uint64_t C8_SQ_BB = 1ULL << 58;
	inline static constexpr uint64_t D8_SQ_BB = 1ULL << 59;
	inline static constexpr uint64_t E8_SQ_BB = 1ULL << 60;
	inline static constexpr uint64_t F8_SQ_BB = 1ULL << 61;
	inline static constexpr uint64_t G8_SQ_BB = 1ULL << 62;
	inline static constexpr uint64_t H8_SQ_BB = 1ULL << 63;

	// Square indices
	inline static constexpr int A1_SQ = 0;
	inline static constexpr int B1_SQ = 1;
	inline static constexpr int C1_SQ = 2;
	inline static constexpr int D1_SQ = 3;
	inline static constexpr int E1_SQ = 4;
	inline static constexpr int F1_SQ = 5;
	inline static constexpr int G1_SQ = 6;
	inline static constexpr int H1_SQ = 7;

	inline static constexpr int A8_SQ = 56;
	inline static constexpr int B8_SQ = 57;
	inline static constexpr int C8_SQ = 58;
	inline static constexpr int D8_SQ = 59;
	inline static constexpr int E8_SQ = 60;
	inline static constexpr int F8_SQ = 61;
	inline static constexpr int G8_SQ = 62;
	inline static constexpr int H8_SQ = 63;

	// Castling rights bits
	inline static constexpr uint8_t CASTLE_WK_BIT = 1 << 0;
	inline static constexpr uint8_t CASTLE_WQ_BIT = 1 << 1;
	inline static constexpr uint8_t CASTLE_BK_BIT = 1 << 2;
	inline static constexpr uint8_t CASTLE_BQ_BIT = 1 << 3;
	// ========================================================================
	// Runtime Static Tables (use std::array for clarity)
	// ========================================================================
	inline static std::array<uint64_t, 64> knight_attacks{};
	inline static std::array<uint64_t, 64> king_attacks{};
	inline static std::array<std::array<uint64_t, 64>, 2> pawn_attacks{};
	inline static bool tables_initialized = false;

	// ========================================================================
	// Intrinsic-Accelerated Functions
	// ========================================================================
	inline static constexpr void set_bit(uint64_t &b, int idx) noexcept
	{
		b |= (1ULL << idx);
	}
	inline static constexpr void clear_bit(uint64_t &b, int idx) noexcept
	{
		b &= ~(1ULL << idx);
	}
	inline static constexpr bool test_bit(uint64_t b, int idx) noexcept
	{
		return (b & (1ULL << idx)) != 0ULL;
	}

	// intrinsics / helpers
	static uint8_t get_lsb_index(uint64_t b) noexcept;
	static uint8_t get_msb_index(uint64_t b) noexcept;
	static uint8_t count_set_bits(uint64_t b) noexcept;
	static uint8_t pop_bit(uint64_t &b) noexcept;

	// convenience: returns indices of set bits (lsb->msb). Now declared.
	static std::vector<int> get_set_bits(uint64_t bitboard);

	// ========================================================================
	// Conversions
	// ========================================================================
	// fast inline conversions
	inline static constexpr uint8_t square_to_file(int sq) noexcept
	{
		return sq & 7;
	}
	inline static constexpr uint8_t square_to_rank(int sq) noexcept
	{
		return sq >> 3;
	}
	inline static constexpr int rank_file_to_square(uint8_t r, uint8_t f) noexcept
	{
		return (r << 3) + f;
	}

	// string helpers
	static std::string square_to_string(int square_idx);
	static std::string move_to_string(const Move &move);

	// Attacks (sliding)
	static uint64_t get_rook_attacks(int square, uint64_t occupancy);
	static uint64_t get_bishop_attacks(int square, uint64_t occupancy);

	// Attack checks
	static bool is_pawn_attacked_by(int target_sq, uint64_t pawn_attackers_bb, PlayerColor attacking_color);
	static bool is_knight_attacked_by(int target_sq, uint64_t knight_attackers_bb);
	static bool is_king_attacked_by(int target_sq, uint64_t king_attackers_bb);
	static bool is_rook_queen_attacked_by(int target_sq, uint64_t rook_queen_attackers_bb, uint64_t occupied_bb);
	static bool is_bishop_queen_attacked_by(int target_sq, uint64_t bishop_queen_attackers_bb, uint64_t occupied_bb);

	// ========================================================================
	// Attack Table Generators
	// ========================================================================
	static void initialize_attack_tables();
	static uint64_t generate_knight_attacks(int sq);
	static uint64_t generate_king_attacks(int sq);
	static uint64_t generate_pawn_attacks(int sq, PlayerColor color);
};
