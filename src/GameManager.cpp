#include "GameManager.h"
#include "ChessBitboardUtils.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

namespace GameManager
{

void run()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::string line;
	while (std::getline(std::cin, line))
	{
		std::stringstream ss(line);
		std::string command;
		ss >> command;

		if (command == "uci")
			handleUciCommand();
		else if (command == "isready")
			handleIsReadyCommand();
		else if (command == "ucinewgame")
			handleUciNewGameCommand();
		else if (command == "position")
			handlePositionCommand(line);
		else if (command == "go")
			handleGoCommand();
		else if (command == "quit")
			break;
		else if (command == "d")
			break;
	}
}

void handleUciCommand()
{
	uci_handler.sendUciIdentity();
	uci_handler.sendUciOk();
}

void handleIsReadyCommand()
{
	uci_handler.sendReadyOk();
}

void handleUciNewGameCommand()
{
	board.reset_to_start_position();
}

void handlePositionCommand(const std::string &command_line)
{
	std::stringstream ss(command_line);
	std::string token, sub_command, current_token_after_board_setup;

	ss >> token >> sub_command;

	if (sub_command == "startpos")
	{
		board.reset_to_start_position();
		ss >> current_token_after_board_setup;
	}
	else if (sub_command == "fen")
	{
		std::string fen_string_builder;
		int fen_components_read = 0;
		while (ss >> current_token_after_board_setup &&
		       current_token_after_board_setup != "moves" &&
		       fen_components_read < 6)
		{
			if (!fen_string_builder.empty())
				fen_string_builder += " ";
			fen_string_builder += current_token_after_board_setup;
			fen_components_read++;
		}
		board.set_from_fen(fen_string_builder);
	}
	else
	{
		std::cerr << "DEBUG: Invalid position command: " << sub_command << std::endl;
		return;
	}

	if (current_token_after_board_setup == "moves")
	{
		MoveGenerator move_gen_local;
		std::string move_str;
		while (ss >> move_str)
		{
			std::vector<Move> current_legal_moves = move_gen_local.generate_legal_moves(board);

			bool move_found = false;
			Move found_move({0, 0}, {0, 0}, PieceTypeIndex::NONE);

			for (const auto &legal_move : current_legal_moves)
			{
				if (ChessBitboardUtils::move_to_string(legal_move) == move_str)
				{
					found_move = legal_move;
					move_found = true;
					break;
				}
			}

			if (move_found)
			{
				StateInfo info_for_undo;
				board.apply_move(found_move, info_for_undo);
			}
			else
			{
				std::cerr << "DEBUG: Invalid move encountered in 'position moves': "
				          << move_str << std::endl;
				std::cerr << "DEBUG: Current FEN: " << board.to_fen() << std::endl;
				break;
			}
		}
	}
}

void handleGoCommand()
{
	Move best_move = chess_ai.findBestMove(board);

	if (best_move.piece_moved_type_idx == PieceTypeIndex::NONE)
		uci_handler.sendBestMove("(none)");
	else
		uci_handler.sendBestMove(ChessBitboardUtils::move_to_string(best_move));
}

} // namespace GameManager
