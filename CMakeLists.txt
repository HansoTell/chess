cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX VERSION 1.0)

# C++ Standard setzen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ausgabeverzeichnis für die Executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include-Pfade für deine Header-Dateien
include_directories(
    ${PROJECT_SOURCE_DIR}/src/Header
    ${PROJECT_SOURCE_DIR}/include
)

# Executable mit allen CPP-Dateien
add_executable(chess
    src/CPP/Main.cpp
    src/CPP/Board.cpp
    src/CPP/GameFigure.cpp
    src/CPP/Move.cpp
    src/CPP/BoardView.cpp
    src/CPP/MovmentLogic.cpp
)

