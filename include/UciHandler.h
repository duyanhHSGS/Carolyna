#ifndef UCI_HANDLER_H
#define UCI_HANDLER_H

#include <iostream>
#include <sstream>
#include <string>

struct UciHandler {
    UciHandler();
    std::string readLine();
    void sendUciIdentity();
    void sendUciOk();
    void sendReadyOk();
    void sendBestMove(const std::string& move_string, const std::string& ponder_string = "");
    void sendInfo(const std::string& message);
};

#endif  // UCI_HANDLER_H