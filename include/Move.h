#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

struct GamePoint {
    uint8_t x;
    uint8_t y;
};

enum class PieceTypeIndex {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    NONE = 6
};

struct Move {
    GamePoint from_square;
    GamePoint to_square;

    PieceTypeIndex piece_moved_type_idx;
    PieceTypeIndex piece_captured_type_idx;

    bool is_promotion;
    PieceTypeIndex promotion_piece_type_idx;

    bool is_kingside_castle;
    bool is_queenside_castle;

    bool is_en_passant;

    bool is_double_pawn_push;

    Move(GamePoint from, GamePoint to,
         PieceTypeIndex moved_type, PieceTypeIndex captured_type = PieceTypeIndex::NONE,
         bool promotion = false, PieceTypeIndex promo_type = PieceTypeIndex::NONE,
         bool k_castle = false, bool q_castle = false,
         bool en_pass = false, bool double_push = false)
        : from_square(from), to_square(to), piece_moved_type_idx(moved_type), piece_captured_type_idx(captured_type), is_promotion(promotion), promotion_piece_type_idx(promo_type), is_kingside_castle(k_castle), is_queenside_castle(q_castle), is_en_passant(en_pass), is_double_pawn_push(double_push) {}
};

#endif  // MOVE_H