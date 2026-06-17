#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <array>
#include <vector>

#include "ChessBoard.h"
#include "Move.h"

const int MAX_MOVES = 256;

struct MoveGenerator {
    std::vector<Move> generate_legal_moves(ChessBoard& board);

    void generate_pawn_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);
    void generate_knight_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);
    void generate_bishop_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);
    void generate_rook_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);
    void generate_queen_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);
    void generate_king_moves(const ChessBoard& board, int square_idx, std::vector<Move>& pseudo_legal_moves);

   private:
    void generate_sliding_piece_moves_helper(const ChessBoard& board, int square_idx, PieceTypeIndex piece_type, std::vector<Move>& pseudo_legal_moves);

    bool is_square_attacked(int square_idx, PlayerColor attacking_color, const ChessBoard& board);
};

#endif  // MOVE_GENERATOR_H