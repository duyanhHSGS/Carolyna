#include "ChessBitboardUtils.h"
#include "MagicTables.h" // DO NOT TOUCH
#include "Move.h"
#include <iostream>
#include <stdexcept>

// ============================================================================
// Attack Table Initialization
// ============================================================================

void ChessBitboardUtils::initialize_attack_tables()
{
	if (tables_initialized)
		return;

	for (int sq = 0; sq < 64; ++sq)
	{
		knight_attacks[sq] = generate_knight_attacks(sq);
		king_attacks[sq] = generate_king_attacks(sq);
		pawn_attacks[static_cast<int>(PlayerColor::White)][sq] = generate_pawn_attacks(sq, PlayerColor::White);
		pawn_attacks[static_cast<int>(PlayerColor::Black)][sq] = generate_pawn_attacks(sq, PlayerColor::Black);
	}

	tables_initialized = true;
}

// ============================================================================
// Attack Table Generation
// ============================================================================

uint64_t ChessBitboardUtils::generate_knight_attacks(int square_idx)
{
	uint64_t attacks = 0ULL;
	int rank = square_idx >> 3;
	int file = square_idx & 7;

	constexpr int dr[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
	constexpr int df[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

	for (int i = 0; i < 8; ++i)
	{
		int nr = rank + dr[i];
		int nf = file + df[i];
		if (nr >= 0 && nr < 8 && nf >= 0 && nf < 8)
			attacks |= 1ULL << (nr * 8 + nf);
	}
	return attacks;
}

uint64_t ChessBitboardUtils::generate_king_attacks(int square_idx)
{
	uint64_t attacks = 0ULL;
	uint64_t king_bb = 1ULL << square_idx;

	// careful with wrap-around on FILE_A / FILE_H
	attacks |= (king_bb << 1) & ~FILE_A; // east
	attacks |= (king_bb >> 1) & ~FILE_H; // west
	attacks |= (king_bb << 8);           // north
	attacks |= (king_bb >> 8);           // south
	attacks |= (king_bb << 9) & ~FILE_A; // north-east
	attacks |= (king_bb << 7) & ~FILE_H; // north-west
	attacks |= (king_bb >> 7) & ~FILE_A; // south-east
	attacks |= (king_bb >> 9) & ~FILE_H; // south-west
	return attacks;
}

uint64_t ChessBitboardUtils::generate_pawn_attacks(int square_idx, PlayerColor color)
{
	uint64_t attacks = 0ULL;
	uint64_t pawn_bb = 1ULL << square_idx;

	if (color == PlayerColor::White)
	{
		attacks |= (pawn_bb << 9) & ~FILE_A;
		attacks |= (pawn_bb << 7) & ~FILE_H;
	}
	else
	{
		attacks |= (pawn_bb >> 9) & ~FILE_H;
		attacks |= (pawn_bb >> 7) & ~FILE_A;
	}
	return attacks;
}

// ============================================================================
// Bit Manipulation (Intrinsics Powered)
// ============================================================================

uint8_t ChessBitboardUtils::get_lsb_index(uint64_t b) noexcept
{
	if (!b)
		return 64;
#if defined(HAS_MSVC_INTRINSICS)
	unsigned long idx;
	_BitScanForward64(&idx, b);
	return static_cast<uint8_t>(idx);
#elif defined(HAS_BUILTIN_CTZLL)
	return static_cast<uint8_t>(__builtin_ctzll(b));
#else
	uint8_t i = 0;
	while ((b & 1) == 0)
	{
		b >>= 1;
		++i;
	}
	return i;
#endif
}

uint8_t ChessBitboardUtils::get_msb_index(uint64_t b) noexcept
{
	if (!b)
		return 64;
#if defined(HAS_MSVC_INTRINSICS)
	unsigned long idx;
	_BitScanReverse64(&idx, b);
	return static_cast<uint8_t>(idx);
#elif defined(HAS_BUILTIN_CLZLL)
	return static_cast<uint8_t>(63 - __builtin_clzll(b));
#else
	uint8_t i = 63;
	while (!((b >> i) & 1))
		--i;
	return i;
#endif
}

uint8_t ChessBitboardUtils::pop_bit(uint64_t &b) noexcept
{
	if (!b)
		return 64;
#if defined(HAS_MSVC_INTRINSICS)
	unsigned long idx;
	_BitScanForward64(&idx, b);
	// _blsr_u64 needs BMI1 (safe on modern CPUs)
	b = _blsr_u64(b);
	return static_cast<uint8_t>(idx);
#elif defined(HAS_BUILTIN_CTZLL)
	uint8_t idx = static_cast<uint8_t>(__builtin_ctzll(b));
	b &= (b - 1);
	return idx;
#else
	uint8_t idx = get_lsb_index(b);
	clear_bit(b, idx);
	return idx;
#endif
}

uint8_t ChessBitboardUtils::count_set_bits(uint64_t b) noexcept
{
#if defined(HAS_MSVC_INTRINSICS)
	return static_cast<uint8_t>(__popcnt64(b));
#elif defined(HAS_BUILTIN_POPCOUNTLL)
	return static_cast<uint8_t>(__builtin_popcountll(b));
#else
	uint8_t cnt = 0;
	while (b)
	{
		b &= (b - 1);
		++cnt;
	}
	return cnt;
#endif
}

std::vector<int> ChessBitboardUtils::get_set_bits(uint64_t b)
{
	std::vector<int> indices;
	indices.reserve(count_set_bits(b));
	while (b)
		indices.push_back(pop_bit(b));
	return indices;
}

// ============================================================================
// Conversions
// ============================================================================

std::string ChessBitboardUtils::square_to_string(int square_idx)
{
	if (square_idx < 0 || square_idx >= 64)
		return "??";
	char file = 'a' + square_to_file(square_idx);
	char rank = '1' + square_to_rank(square_idx);
	return {file, rank};
}

std::string ChessBitboardUtils::move_to_string(const Move &move)
{
	std::string out;
	out += square_to_string(rank_file_to_square(move.from_square.y, move.from_square.x));
	out += square_to_string(rank_file_to_square(move.to_square.y, move.to_square.x));

	if (move.is_promotion)
	{
		switch (move.promotion_piece_type_idx)
		{
		case PieceTypeIndex::QUEEN:
			out += 'q';
			break;
		case PieceTypeIndex::ROOK:
			out += 'r';
			break;
		case PieceTypeIndex::BISHOP:
			out += 'b';
			break;
		case PieceTypeIndex::KNIGHT:
			out += 'n';
			break;
		default:
			break;
		}
	}
	return out;
}

// ============================================================================
// Sliding Piece Attacks (Magic Bitboards)
// ============================================================================

uint64_t ChessBitboardUtils::get_rook_attacks(int square, uint64_t occupancy)
{
	const MagicEntry &m = rook_magics[square];
	return m.attacks[((occupancy & m.mask) * m.magic) >> m.shift];
}

uint64_t ChessBitboardUtils::get_bishop_attacks(int square, uint64_t occupancy)
{
	const MagicEntry &m = bishop_magics[square];
	return m.attacks[((occupancy & m.mask) * m.magic) >> m.shift];
}

// ============================================================================
// Attack Detection
// ============================================================================

bool ChessBitboardUtils::is_pawn_attacked_by(int target_sq, uint64_t pawn_attackers_bb, PlayerColor attacking_color)
{
	// Opposite color because we want where an enemy pawn would *have come from*
	uint64_t attacks_from_target =
	    pawn_attacks[static_cast<int>(attacking_color == PlayerColor::White ? PlayerColor::Black : PlayerColor::White)][target_sq];
	return (attacks_from_target & pawn_attackers_bb) != 0ULL;
}

bool ChessBitboardUtils::is_knight_attacked_by(int target_sq, uint64_t knight_attackers_bb)
{
	return (knight_attacks[target_sq] & knight_attackers_bb) != 0ULL;
}

bool ChessBitboardUtils::is_king_attacked_by(int target_sq, uint64_t king_attackers_bb)
{
	return (king_attacks[target_sq] & king_attackers_bb) != 0ULL;
}

bool ChessBitboardUtils::is_rook_queen_attacked_by(int target_sq, uint64_t rook_queen_attackers_bb, uint64_t occupied_bb)
{
	uint64_t attacks = get_rook_attacks(target_sq, occupied_bb);
	return (attacks & rook_queen_attackers_bb) != 0ULL;
}

bool ChessBitboardUtils::is_bishop_queen_attacked_by(int target_sq, uint64_t bishop_queen_attackers_bb, uint64_t occupied_bb)
{
	uint64_t attacks = get_bishop_attacks(target_sq, occupied_bb);
	return (attacks & bishop_queen_attackers_bb) != 0ULL;
}
