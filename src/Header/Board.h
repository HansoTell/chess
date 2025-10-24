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

using json = nlohmann::json;
namespace chess
{
    class Board
    {
    private:
        std::array<GameFigure*, boardSize> m_BoardPositions;
        std::vector<GameFigure> m_Figures;
        const BoardView m_BoardView;
    public:

        Board(const std::string& filename);
        Board(Board& other) = delete;
        ~Board() = default;

        void printBoard() const;
        bool makeMove(const Move& move);
        bool checkCheckmate(Color playerColor) const;

    private:
        void boardinit(const std::string& file);
        json parseJson(const std::string& file);
        void fillBoard(const json& jsonMap);
        bool isMoveLegal(const Move& move) const;
        bool checkCheck(const Move& move) const;
        void executeMove(const Move& move);
    };
}




