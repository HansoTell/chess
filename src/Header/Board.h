#pragma once
#include<iostream>
#include <array>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
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
        void updateThreatendSquares(std::optional<GameFigure>& capturedFigure, const Move& move);
        void removeOldThreats(GameFigure* figure);
        void refreshThreats(GameFigure* figure);

        void boardinit(const json& gameConfig);
        void updateGameState(std::optional<GameFigure>& capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType);
        std::optional<GameFigure> editBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move);

        MoveResult isMoveLegal(const Move& move) const;
        bool isInCheck(Color color) const;
        bool wouldBeInCheck() const;

        std::optional<GameFigure> executeMove(const Move& move, MoveResult moveresult);
    };
}




