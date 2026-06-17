#include "Evaluation.h"

#include "ChessAI.h"
#include "Constants.h"

namespace Evaluation {
int calculate_pawn_shield_penalty_internal(const ChessBoard& board_ref, PlayerColor king_color,
                                           uint64_t friendly_pawns_bb) {
    int penalty = 0;

    const uint64_t WHITE_KINGSIDE_KING_ZONE = (1ULL << ChessBitboardUtils::F1_SQ) | (1ULL << ChessBitboardUtils::G1_SQ) | (1ULL << ChessBitboardUtils::H1_SQ);
    const uint64_t WHITE_QUEENSIDE_KING_ZONE = (1ULL << ChessBitboardUtils::A1_SQ) | (1ULL << ChessBitboardUtils::B1_SQ) | (1ULL << ChessBitboardUtils::C1_SQ);
    const uint64_t BLACK_KINGSIDE_KING_ZONE = (1ULL << ChessBitboardUtils::F8_SQ) | (1ULL << ChessBitboardUtils::G8_SQ) | (1ULL << ChessBitboardUtils::H8_SQ);
    const uint64_t BLACK_QUEENSIDE_KING_ZONE = (1ULL << ChessBitboardUtils::A8_SQ) | (1ULL << ChessBitboardUtils::B8_SQ) | (1ULL << ChessBitboardUtils::C8_SQ);

    if (king_color == PlayerColor::White) {
        if ((board_ref.white_king & WHITE_KINGSIDE_KING_ZONE) != 0ULL) {
            uint64_t white_kingside_shield_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(1, 5)) |
                                                  (1ULL << ChessBitboardUtils::rank_file_to_square(1, 6)) |
                                                  (1ULL << ChessBitboardUtils::rank_file_to_square(1, 7));
            uint64_t missing_white_kingside_pawns = white_kingside_shield_mask & (~friendly_pawns_bb);
            penalty -= ChessBitboardUtils::count_set_bits(missing_white_kingside_pawns) * PAWN_SHIELD_MISSING_PAWN_PENALTY;

            uint64_t white_kingside_advanced_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(2, 5)) |
                                                    (1ULL << ChessBitboardUtils::rank_file_to_square(2, 6)) |
                                                    (1ULL << ChessBitboardUtils::rank_file_to_square(2, 7));
            uint64_t advanced_white_kingside_pawns = friendly_pawns_bb & white_kingside_advanced_mask;
            penalty -= ChessBitboardUtils::count_set_bits(advanced_white_kingside_pawns) * PAWN_SHIELD_ADVANCED_PAWN_PENALTY;
        }
        if ((board_ref.white_king & WHITE_QUEENSIDE_KING_ZONE) != 0ULL) {
            uint64_t white_queenside_shield_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(1, 0)) |
                                                   (1ULL << ChessBitboardUtils::rank_file_to_square(1, 1)) |
                                                   (1ULL << ChessBitboardUtils::rank_file_to_square(1, 2));
            uint64_t missing_white_queenside_pawns = white_queenside_shield_mask & (~friendly_pawns_bb);
            penalty -= ChessBitboardUtils::count_set_bits(missing_white_queenside_pawns) * PAWN_SHIELD_MISSING_PAWN_PENALTY;

            uint64_t white_queenside_advanced_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(2, 0)) |
                                                     (1ULL << ChessBitboardUtils::rank_file_to_square(2, 1)) |
                                                     (1ULL << ChessBitboardUtils::rank_file_to_square(2, 2));
            uint64_t advanced_white_queenside_pawns = friendly_pawns_bb & white_queenside_advanced_mask;
            penalty -= ChessBitboardUtils::count_set_bits(advanced_white_queenside_pawns) * PAWN_SHIELD_ADVANCED_PAWN_PENALTY;
        }
    } else {
        if ((board_ref.black_king & BLACK_KINGSIDE_KING_ZONE) != 0ULL) {
            uint64_t black_kingside_shield_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(6, 5)) |
                                                  (1ULL << ChessBitboardUtils::rank_file_to_square(6, 6)) |
                                                  (1ULL << ChessBitboardUtils::rank_file_to_square(6, 7));
            uint64_t missing_black_kingside_pawns = black_kingside_shield_mask & (~friendly_pawns_bb);
            penalty -= ChessBitboardUtils::count_set_bits(missing_black_kingside_pawns) * PAWN_SHIELD_MISSING_PAWN_PENALTY;

            uint64_t black_kingside_advanced_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(5, 5)) |
                                                    (1ULL << ChessBitboardUtils::rank_file_to_square(5, 6)) |
                                                    (1ULL << ChessBitboardUtils::rank_file_to_square(5, 7));
            uint64_t advanced_black_kingside_pawns = friendly_pawns_bb & black_kingside_advanced_mask;
            penalty -= ChessBitboardUtils::count_set_bits(advanced_black_kingside_pawns) * PAWN_SHIELD_ADVANCED_PAWN_PENALTY;
        }
        if ((board_ref.black_king & BLACK_QUEENSIDE_KING_ZONE) != 0ULL) {
            uint64_t black_queenside_shield_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(6, 0)) |
                                                   (1ULL << ChessBitboardUtils::rank_file_to_square(6, 1)) |
                                                   (1ULL << ChessBitboardUtils::rank_file_to_square(6, 2));
            uint64_t missing_black_queenside_pawns = black_queenside_shield_mask & (~friendly_pawns_bb);
            penalty -= ChessBitboardUtils::count_set_bits(missing_black_queenside_pawns) * PAWN_SHIELD_MISSING_PAWN_PENALTY;

            uint64_t black_queenside_advanced_mask = (1ULL << ChessBitboardUtils::rank_file_to_square(5, 0)) |
                                                     (1ULL << ChessBitboardUtils::rank_file_to_square(5, 1)) |
                                                     (1ULL << ChessBitboardUtils::rank_file_to_square(5, 2));
            uint64_t advanced_black_queenside_pawns = friendly_pawns_bb & black_queenside_advanced_mask;
            penalty -= ChessBitboardUtils::count_set_bits(advanced_black_queenside_pawns) * PAWN_SHIELD_ADVANCED_PAWN_PENALTY;
        }
    }
    return penalty;
}

int calculate_open_file_penalty_internal(int king_square,
                                         uint64_t friendly_pawns_bb, uint64_t enemy_pawns_bb) {
    int penalty = 0;
    int king_file = ChessBitboardUtils::square_to_file(king_square);

    for (int f_offset = -1; f_offset <= 1; ++f_offset) {
        int current_file = king_file + f_offset;
        if (current_file >= 0 && current_file < 8) {
            uint64_t file_mask = FILE_MASKS_ARRAY[current_file];
            uint64_t file_contents = friendly_pawns_bb | enemy_pawns_bb;

            if (!((file_contents & file_mask) != 0ULL)) {
                penalty -= OPEN_FILE_FULL_OPEN_PENALTY;
            } else if (!((friendly_pawns_bb & file_mask) != 0ULL)) {
                penalty -= OPEN_FILE_SEMI_OPEN_PENALTY;
            }
        }
    }
    return penalty;
}

int evaluate(const ChessBoard& board) {
    int score = 0;

    for (int i = 0; i < 64; ++i) {
        if (ChessBitboardUtils::test_bit(board.white_pawns, i))
            score += (PAWN_VALUE + ChessAI::PAWN_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.white_knights, i))
            score += (KNIGHT_VALUE + ChessAI::KNIGHT_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.white_bishops, i))
            score += (BISHOP_VALUE + ChessAI::BISHOP_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.white_rooks, i))
            score += (ROOK_VALUE + ChessAI::ROOK_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.white_queens, i))
            score += (QUEEN_VALUE + ChessAI::QUEEN_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.white_king, i))
            score += (KING_VALUE + ChessAI::KING_PST[i]);
        else if (ChessBitboardUtils::test_bit(board.black_pawns, i))
            score -= (PAWN_VALUE + ChessAI::PAWN_PST[63 - i]);
        else if (ChessBitboardUtils::test_bit(board.black_knights, i))
            score -= (KNIGHT_VALUE + ChessAI::KNIGHT_PST[63 - i]);
        else if (ChessBitboardUtils::test_bit(board.black_bishops, i))
            score -= (BISHOP_VALUE + ChessAI::BISHOP_PST[63 - i]);
        else if (ChessBitboardUtils::test_bit(board.black_rooks, i))
            score -= (ROOK_VALUE + ChessAI::ROOK_PST[63 - i]);
        else if (ChessBitboardUtils::test_bit(board.black_queens, i))
            score -= (QUEEN_VALUE + ChessAI::QUEEN_PST[63 - i]);
        else if (ChessBitboardUtils::test_bit(board.black_king, i))
            score -= (KING_VALUE + ChessAI::KING_PST[63 - i]);
    }

    int white_pawn_structure_score = 0;
    int black_pawn_structure_score = 0;

    uint64_t white_doubled_files_penalized = 0ULL;
    uint64_t black_doubled_files_penalized = 0ULL;

    uint64_t current_white_pawns_bb = board.white_pawns;
    while (current_white_pawns_bb) {
        int pawn_sq_idx = ChessBitboardUtils::get_lsb_index(current_white_pawns_bb);
        int file = ChessBitboardUtils::square_to_file(pawn_sq_idx);
        int rank = ChessBitboardUtils::square_to_rank(pawn_sq_idx);

        uint64_t adjacent_files_mask = 0ULL;
        if (file > 0) {
            adjacent_files_mask |= FILE_MASKS_ARRAY[file - 1];
        }
        if (file < 7) {
            adjacent_files_mask |= FILE_MASKS_ARRAY[file + 1];
        }
        if ((board.white_pawns & adjacent_files_mask) == 0ULL) {
            white_pawn_structure_score -= ISOLATED_PAWN_PENALTY;
        }

        uint64_t current_file_mask = FILE_MASKS_ARRAY[file];
        if (!ChessBitboardUtils::test_bit(white_doubled_files_penalized, file)) {
            if (ChessBitboardUtils::count_set_bits(board.white_pawns & current_file_mask) > 1) {
                white_pawn_structure_score -= DOUBLED_PAWN_PENALTY;
                white_doubled_files_penalized |= (1ULL << file);
            }
        }

        uint64_t white_passed_pawn_mask = 0ULL;
        for (int r = rank + 1; r < 8; ++r) {
            white_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file));
            if (file > 0) {
                white_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file - 1));
            }
            if (file < 7) {
                white_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file + 1));
            }
        }
        if ((board.black_pawns & white_passed_pawn_mask) == 0ULL) {
            int passed_pawn_bonus = PASSED_PAWN_BASE_BONUS;
            passed_pawn_bonus += (rank - 1) * PASSED_PAWN_RANK_BONUS_FACTOR;
            white_pawn_structure_score += passed_pawn_bonus;
        }

        uint64_t squares_attacking_this_pawn_mask = 0ULL;
        if (file > 0 && rank > 0) {
            squares_attacking_this_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(rank - 1, file - 1));
        }
        if (file < 7 && rank > 0) {
            squares_attacking_this_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(rank - 1, file + 1));
        }
        if ((board.white_pawns & squares_attacking_this_pawn_mask) != 0ULL) {
            white_pawn_structure_score += CONNECTED_PAWN_BONUS;
        }

        current_white_pawns_bb &= (current_white_pawns_bb - 1);
    }

    uint64_t current_black_pawns_bb = board.black_pawns;
    while (current_black_pawns_bb) {
        int pawn_sq_idx = ChessBitboardUtils::get_lsb_index(current_black_pawns_bb);
        int file = ChessBitboardUtils::square_to_file(pawn_sq_idx);
        int rank = ChessBitboardUtils::square_to_rank(pawn_sq_idx);

        uint64_t adjacent_files_mask = 0ULL;
        if (file > 0) {
            adjacent_files_mask |= FILE_MASKS_ARRAY[file - 1];
        }
        if (file < 7) {
            adjacent_files_mask |= FILE_MASKS_ARRAY[file + 1];
        }
        if ((board.black_pawns & adjacent_files_mask) == 0ULL) {
            black_pawn_structure_score -= ISOLATED_PAWN_PENALTY;
        }

        uint64_t current_file_mask = FILE_MASKS_ARRAY[file];
        if (!ChessBitboardUtils::test_bit(black_doubled_files_penalized, file)) {
            if (ChessBitboardUtils::count_set_bits(board.black_pawns & current_file_mask) > 1) {
                black_pawn_structure_score -= DOUBLED_PAWN_PENALTY;
                black_doubled_files_penalized |= (1ULL << file);
            }
        }

        uint64_t black_passed_pawn_mask = 0ULL;
        for (int r = rank - 1; r >= 0; --r) {
            black_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file));
            if (file > 0) {
                black_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file - 1));
            }
            if (file < 7) {
                black_passed_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(r, file + 1));
            }
        }
        if ((board.white_pawns & black_passed_pawn_mask) == 0ULL) {
            int passed_pawn_bonus = PASSED_PAWN_BASE_BONUS;
            passed_pawn_bonus += (6 - rank) * PASSED_PAWN_RANK_BONUS_FACTOR;
            black_pawn_structure_score += passed_pawn_bonus;
        }

        uint64_t squares_attacking_this_pawn_mask = 0ULL;
        if (file > 0 && rank < 7) {
            squares_attacking_this_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(rank + 1, file - 1));
        }
        if (file < 7 && rank < 7) {
            squares_attacking_this_pawn_mask |= (1ULL << ChessBitboardUtils::rank_file_to_square(rank + 1, file + 1));
        }
        if ((board.black_pawns & squares_attacking_this_pawn_mask) != 0ULL) {
            black_pawn_structure_score += CONNECTED_PAWN_BONUS;
        }

        current_black_pawns_bb &= (current_black_pawns_bb - 1);
    }

    score += white_pawn_structure_score;
    score -= black_pawn_structure_score;

    int white_king_safety_score = 0;
    int black_king_safety_score = 0;

    int white_king_sq = ChessBitboardUtils::get_lsb_index(board.white_king);
    int black_king_sq = ChessBitboardUtils::get_lsb_index(board.black_king);

    white_king_safety_score += calculate_pawn_shield_penalty_internal(board, PlayerColor::White, board.white_pawns);
    black_king_safety_score += calculate_pawn_shield_penalty_internal(board, PlayerColor::Black, board.black_pawns);

    if (!((board.castling_rights_mask & (1 << 3)) != 0) &&
        (board.white_king & (1ULL << ChessBitboardUtils::G1_SQ)) != 0ULL) {
        white_king_safety_score += CASTLING_BONUS_KINGSIDE;
    }
    if (!((board.castling_rights_mask & (1 << 2)) != 0) &&
        (board.white_king & (1ULL << ChessBitboardUtils::C1_SQ)) != 0ULL) {
        white_king_safety_score += CASTLING_BONUS_QUEENSIDE;
    }

    if (!((board.castling_rights_mask & (1 << 1)) != 0) &&
        (board.black_king & (1ULL << ChessBitboardUtils::G8_SQ)) != 0ULL) {
        black_king_safety_score += CASTLING_BONUS_KINGSIDE;
    }
    if (!((board.castling_rights_mask & (1 << 0)) != 0) &&
        (board.black_king & (1ULL << ChessBitboardUtils::C8_SQ)) != 0ULL) {
        black_king_safety_score += CASTLING_BONUS_QUEENSIDE;
    }

    white_king_safety_score += calculate_open_file_penalty_internal(white_king_sq, board.white_pawns, board.black_pawns);
    black_king_safety_score += calculate_open_file_penalty_internal(black_king_sq, board.black_pawns, board.white_pawns);

    score += white_king_safety_score;
    score -= black_king_safety_score;

    int white_mobility_score = 0;
    int black_mobility_score = 0;

    uint64_t all_occupied_bb = board.occupied_squares;

    uint64_t temp_piece_bb;
    int piece_sq_idx;
    uint64_t attacks_bb;

    temp_piece_bb = board.white_pawns;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::pawn_attacks[static_cast<int>(PlayerColor::White)][piece_sq_idx];

        if (ChessBitboardUtils::square_to_rank(piece_sq_idx) < 7) {
            int one_step_forward_sq = piece_sq_idx + 8;
            if (!ChessBitboardUtils::test_bit(all_occupied_bb, one_step_forward_sq)) {
                attacks_bb |= (1ULL << one_step_forward_sq);

                if (ChessBitboardUtils::square_to_rank(piece_sq_idx) == 1) {
                    int two_steps_forward_sq = piece_sq_idx + 16;
                    if (!ChessBitboardUtils::test_bit(all_occupied_bb, two_steps_forward_sq)) {
                        attacks_bb |= (1ULL << two_steps_forward_sq);
                    }
                }
            }
        }
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.white_knights;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::knight_attacks[piece_sq_idx];
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.white_bishops;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_bishop_attacks(piece_sq_idx, all_occupied_bb);
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.white_rooks;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_rook_attacks(piece_sq_idx, all_occupied_bb);
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.white_queens;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_rook_attacks(piece_sq_idx, all_occupied_bb) |
                     ChessBitboardUtils::get_bishop_attacks(piece_sq_idx, all_occupied_bb);
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.white_king;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::king_attacks[piece_sq_idx];
        white_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_pawns;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::pawn_attacks[static_cast<int>(PlayerColor::Black)][piece_sq_idx];
        if (ChessBitboardUtils::square_to_rank(piece_sq_idx) > 0) {
            int one_step_forward_sq = piece_sq_idx - 8;
            if (!ChessBitboardUtils::test_bit(all_occupied_bb, one_step_forward_sq)) {
                attacks_bb |= (1ULL << one_step_forward_sq);
                if (ChessBitboardUtils::square_to_rank(piece_sq_idx) == 6) {
                    int two_steps_forward_sq = piece_sq_idx - 16;
                    if (!ChessBitboardUtils::test_bit(all_occupied_bb, two_steps_forward_sq)) {
                        attacks_bb |= (1ULL << two_steps_forward_sq);
                    }
                }
            }
        }
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_knights;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::knight_attacks[piece_sq_idx];
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_bishops;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_bishop_attacks(piece_sq_idx, all_occupied_bb);
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_rooks;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_rook_attacks(piece_sq_idx, all_occupied_bb);
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_queens;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::get_rook_attacks(piece_sq_idx, all_occupied_bb) |
                     ChessBitboardUtils::get_bishop_attacks(piece_sq_idx, all_occupied_bb);
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    temp_piece_bb = board.black_king;
    while (temp_piece_bb) {
        piece_sq_idx = ChessBitboardUtils::pop_bit(temp_piece_bb);
        attacks_bb = ChessBitboardUtils::king_attacks[piece_sq_idx];
        black_mobility_score += ChessBitboardUtils::count_set_bits(attacks_bb);
    }

    score += white_mobility_score * MOBILITY_BONUS_PER_SQUARE;
    score -= black_mobility_score * MOBILITY_BONUS_PER_SQUARE;

    return score;
}

}  // namespace Evaluation