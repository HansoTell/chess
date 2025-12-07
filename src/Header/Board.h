#pragma once
#include<iostream>
#include <array>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <variant>
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
        void boardinit(const json& gameConfig);
            void addFigureOffJson(const json& posData, FigureType figureType, Color color);

        MoveResult isMoveLegal(const Move& move) const;
        bool isInCheck(Color color) const;
        bool wouldBeInCheck(const Move& move);
        void updateGameState(const GameFigure* capturedFigure, const Move& move, std::optional<MoveType> moveType, FigureType movedFigureType);

        void threatendSquaresInit();
        void updateThreatendSquares(const GameFigure* capturedFigure, const Move& move);
            void removeOldThreats(const GameFigure* figure);
            void refreshThreats(GameFigure* figure);

        std::optional<GameFigure> executeMove(const Move& move, MoveResult moveresult, std::optional<FigureType> promotedFigureType);
            std::optional<GameFigure> ExecuteNormalMove(const Move& move);
            std::optional<GameFigure>ExecuteCastelingMove(const Move& move);
            std::optional<GameFigure> ExecuteEnPassantMove(const Move& move);
            std::optional<GameFigure>ExecutePromotingMove(const Move& move, FigureType promotedFigureType);
        std::optional<GameFigure> editBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move);


        CachedThreats simulateUpdateThreatendSquares(const GameFigure* capturedFigure, const Move& move, bool caching = false);
            void simulateRemoveOldThreats(const GameFigure* figure, CachedThreats& cachedThreats);
            void simulateRefreshThreats(GameFigure* figure, CachedThreats& cachedThreats);

        ChangedPieces simulateMove(const Move& move, MoveResult moveresult, std::optional<FigureType> promotedFigureType);        
            ChangedPieces simulateNormalMove(const Move& move);
            ChangedPieces simulateCastelingMove(const Move& move);
            ChangedPieces simulateEnPassantMove(const Move& move);
            ChangedPieces simulatePromotingMove(const Move& move);
        ChangedPieces simulateEditBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move);
    };
}
