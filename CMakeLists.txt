cmake_minimum_required(VERSION 3.16)
project(Carolyna LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

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

add_executable(carolyna_tests
    tests/EngineSmokeTests.cpp
    src/ChessBitboardUtils.cpp
    src/ChessBoard.cpp
    src/MagicTables.cpp
    src/MoveGenerator.cpp
)

target_compile_definitions(carolyna PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
target_compile_definitions(carolyna_tests PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
target_include_directories(carolyna PRIVATE include)
target_include_directories(carolyna_tests PRIVATE include)

target_compile_features(carolyna PRIVATE cxx_std_23)
target_compile_features(carolyna_tests PRIVATE cxx_std_23)
target_compile_options(carolyna PRIVATE -Wall -Wextra -O3 -funroll-loops -march=native -flto=auto)
target_compile_options(carolyna_tests PRIVATE -Wall -Wextra -O3 -funroll-loops -march=native -flto=auto)

