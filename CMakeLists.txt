cmake_minimum_required(VERSION 3.16)
project(Carolyna LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# Add the executable with all .cpp files
add_executable(carolyna
    src/main.cpp
    src/ChessAI.cpp
    src/ChessBitboardUtils.cpp
    src/ChessBoard.cpp
    src/Evaluation.cpp
    src/GameManager.cpp
    src/MagicTables.cpp
    src/MoveGenerator.cpp
    src/UciHandler.cpp
)

# Include folder for headers
target_include_directories(carolyna PRIVATE include)

# Tags / compile options
target_compile_features(carolyna PRIVATE cxx_std_23)
target_compile_options(carolyna PRIVATE -Wall -Wextra -O3 -funroll-loops -march=native -flto)

