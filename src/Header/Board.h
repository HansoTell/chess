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
#include "BoardPrinter.h"
#include "CachedThreats.h"

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
        std::unique_ptr<BoardPrinter> m_BoardPrinter;
    public:
        Board(const std::string& filename, std::unique_ptr<BoardPrinter> boardPrinter);
        Board(Board& other) = delete;
        ~Board() = default;
    public:
        void printBoard() const { m_BoardPrinter->printBoard(m_BoardView); }
        bool makeMove(const Move& move);
        bool isCheckmate(Color playerColor) const;

    private:
        json parseJson(const std::string& file);

        void gameStateInit(const json& gameConfig);

        void threatendSquaresInit();
        CachedThreats updateThreatendSquares(const GameFigure* capturedFigure, const Move& move, bool caching = false);
        template<typename F>
        void removeOldThreats(const GameFigure* figure, F callback,  bool caching);
        template<typename F>
        void refreshThreats(GameFigure* figure, F callback, bool caching);

        void boardinit(const json& gameConfig);
        void addFigureOffJson(const json& posData, FigureType figureType, Color color);
        void updateGameState(const GameFigure* capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType);
        std::optional<GameFigure> editBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move);

        MoveResult isMoveLegal(const Move& move) const;
        bool isInCheck(Color color) const;
        bool wouldBeInCheck(const Move& move);

        std::optional<GameFigure> executeMove(const Move& move, MoveResult moveresult, std::optional<FigureType> promotedFigureType);
    };
}




