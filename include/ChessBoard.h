#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <cstdint>
#include <iosfwd>
#include <string>

#include "Move.h"

enum class PlayerColor {
    White,
    Black,
    NONE
};

struct StateInfo {
    uint8_t previous_castling_rights_mask;
    int previous_en_passant_square_idx;
    int previous_halfmove_clock;
    int previous_fullmove_number;
    PlayerColor previous_active_player;
    PieceTypeIndex captured_piece_type_idx;
    PlayerColor captured_piece_color;
    int captured_square_idx;

    StateInfo() : previous_castling_rights_mask(0),
                  previous_en_passant_square_idx(64),
                  previous_halfmove_clock(0),
                  previous_fullmove_number(0),
                  previous_active_player(PlayerColor::White),
                  captured_piece_type_idx(PieceTypeIndex::NONE),
                  captured_piece_color(PlayerColor::White),
                  captured_square_idx(64) {}
};

struct ChessBoard {
    uint64_t white_pawns;
    uint64_t white_knights;
    uint64_t white_bishops;
    uint64_t white_rooks;
    uint64_t white_queens;
    uint64_t white_king;

    uint64_t black_pawns;
    uint64_t black_knights;
    uint64_t black_bishops;
    uint64_t black_rooks;
    uint64_t black_queens;
    uint64_t black_king;

    uint64_t occupied_squares;
    uint64_t white_occupied_squares;
    uint64_t black_occupied_squares;

    PlayerColor active_player;

    uint8_t castling_rights_mask;

    int en_passant_square_idx;

    int halfmove_clock;
    int fullmove_number;

    uint64_t zobrist_hash;

    static uint64_t zobrist_piece_keys[12][64];
    static uint64_t zobrist_black_to_move_key;
    static uint64_t zobrist_castling_keys[16];
    static uint64_t zobrist_en_passant_keys[8];
    static bool zobrist_keys_initialized;

    ChessBoard();
    explicit ChessBoard(const std::string& fen);

    void reset_to_start_position();
    void set_from_fen(const std::string& fen);
    std::string to_fen() const;

    void apply_move(const Move& move, StateInfo& state_info);

    void undo_move(const Move& move, const StateInfo& state_info);

    bool is_king_in_check(PlayerColor king_color) const;

    int get_piece_square_index(PieceTypeIndex piece_type_idx, PlayerColor piece_color) const;

#ifdef DEBUG
    bool debug_validate_bitboards() const;
    bool debug_validate_bitboards(std::ostream& out) const;
    void debug_print_bitboards(std::ostream& out) const;
#endif

    static void initialize_zobrist_keys();
    uint64_t calculate_zobrist_hash_from_scratch() const;
    void toggle_zobrist_piece(PieceTypeIndex piece_type_idx, PlayerColor piece_color, int square_idx);
};

#endif  // CHESS_BOARD_H
