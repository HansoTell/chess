#pragma once
#include<iostream>
#include <array>
#include <fstream>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "GameFigure.h"
#include "Move.h"
#include "Constants.h"
#include "Color.h"
#include "BoardView.h"
#include "GameState.h"
#include "MoveResult.h"

using json = nlohmann::json;
namespace chess
{
    class Board
    {
    private:
        std::array<GameFigure*, boardSize> m_BoardPositions;
        std::vector<GameFigure> m_Figures;
        const BoardView m_BoardView;
        GameState m_GameState;
    public:
        Board(const std::string& filename);
        Board(Board& other) = delete;
        ~Board() = default;
    public:
        void printBoard() const;
        bool makeMove(const Move& move);
        bool isCheckmate(Color playerColor) const;

    private:
        json parseJson(const std::string& file);

        void gameStateInit(const json& gameConfig);

        void threatendSquaresInit();
        void updateThreatendSquares();

        void boardinit(const json& gameConfig);

        MoveResult isMoveLegal(const Move& move) const;
        bool isInCheck(const Move& move) const;

        void executeMove(const Move& move);
    };
}




