#ifndef EVALUATION_H
#define EVALUATION_H

#include <array>
#include <cstdint>

#include "ChessAI.h"
#include "ChessBitboardUtils.h"
#include "ChessBoard.h"
#include "Constants.h"

namespace Evaluation {

int calculate_pawn_shield_penalty_internal(const ChessBoard& board_ref, PlayerColor king_color, int king_square, uint64_t friendly_pawns_bb);

int calculate_open_file_penalty_internal(PlayerColor king_color, int king_square, uint64_t friendly_pawns_bb, uint64_t enemy_pawns_bb);

int evaluate(const ChessBoard& board);

}  // namespace Evaluation

#endif  // EVALUATION_H
