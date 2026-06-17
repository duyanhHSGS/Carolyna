#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "ChessBitboardUtils.h"
#include "ChessBoard.h"
#include "MoveGenerator.h"

namespace {

bool contains_legal_move(ChessBoard& board, const std::string& move_string) {
    MoveGenerator generator;
    const std::vector<Move> moves = generator.generate_legal_moves(board);

    for (const Move& move : moves) {
        if (ChessBitboardUtils::move_to_string(move) == move_string) {
            return true;
        }
    }

    return false;
}

bool apply_uci_move(ChessBoard& board, const std::string& move_string) {
    MoveGenerator generator;
    const std::vector<Move> moves = generator.generate_legal_moves(board);

    for (const Move& move : moves) {
        if (ChessBitboardUtils::move_to_string(move) == move_string) {
            StateInfo state;
            board.apply_move(move, state);
            return true;
        }
    }

    std::cerr << "missing legal move: " << move_string << "\n";
    std::cerr << "current fen: " << board.to_fen() << "\n";
    std::cerr << "legal moves:";
    for (const Move& move : moves) {
        std::cerr << ' ' << ChessBitboardUtils::move_to_string(move);
    }
    std::cerr << "\n";
    return false;
}

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        std::exit(1);
    }
}

void test_start_position_has_knight_moves() {
    ChessBoard board;

    expect(contains_legal_move(board, "b1c3"), "start position should contain b1c3");
    expect(contains_legal_move(board, "g1f3"), "start position should contain g1f3");
}

void test_arena_replay_line_accepts_b1c3() {
    ChessBoard board;

    expect(apply_uci_move(board, "e2e4"), "e2e4 should apply");
    expect(apply_uci_move(board, "d7d5"), "d7d5 should apply");
    expect(apply_uci_move(board, "e4d5"), "e4d5 should apply");
    expect(apply_uci_move(board, "d8d5"), "d8d5 should apply");
    expect(apply_uci_move(board, "b1c3"), "b1c3 should apply after Qxd5");

    expect(board.to_fen() == "rnb1kbnr/ppp1pppp/8/3q4/8/2N5/PPPP1PPP/R1BQKBNR b KQkq - 1 3",
           "Arena replay FEN after b1c3 should match expected position");
}

}  // namespace

int main() {
    ChessBitboardUtils::initialize_attack_tables();

    test_start_position_has_knight_moves();
    test_arena_replay_line_accepts_b1c3();

    std::cout << "Engine smoke tests passed\n";
    return 0;
}
