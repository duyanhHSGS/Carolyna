#pragma once

#include <string>
#include <vector>

#include "ChessAI.h"
#include "ChessBoard.h"
#include "Move.h"
#include "MoveGenerator.h"
#include "UciHandler.h"

namespace GameManager
{

// Inline globals = shared across translation units
inline ChessBoard board{};
inline ChessAI chess_ai{};
inline UciHandler uci_handler{};

void run();

void handleUciCommand();
void handleIsReadyCommand();
void handleUciNewGameCommand();
void handlePositionCommand(const std::string &command_line);
void handleGoCommand();
} // namespace GameManager
