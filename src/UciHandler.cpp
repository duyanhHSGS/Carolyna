#include "UciHandler.h"

UciHandler::UciHandler() {
}

std::string UciHandler::readLine() {
    std::string line;
    if (std::getline(std::cin, line)) {
        return line;
    }
    return "";
}

void UciHandler::sendUciIdentity() {
    std::cout << "id name Carolyna" << std::endl;
    std::cout << "id author Duy Anh" << std::endl;
}

void UciHandler::sendUciOk() {
    std::cout << "uciok" << std::endl;
}

void UciHandler::sendReadyOk() {
    std::cout << "readyok" << std::endl;
}

void UciHandler::sendBestMove(const std::string& move_string, const std::string& ponder_string) {
    std::cout << "bestmove " << move_string;
    if (!ponder_string.empty()) {
        std::cout << " ponder " << ponder_string;
    }
    std::cout << std::endl;
}

void UciHandler::sendInfo(const std::string& message) {
    std::cout << "info string " << message << std::endl;
}