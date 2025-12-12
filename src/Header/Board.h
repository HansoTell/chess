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

    //Neue Idee zum move reworken
    //anstatt einen move anzufragen und dann zu prüfen ob er legal ist am anfang der runde alle legalen moves berechnen und in figure speichernm
    //dann einersiets kann gezeiotg werden was überhaupt machbar ist und andererseits anfragen ob move legal ganz einfach gucken ob move in move cache der figur
    //Brauchen:
    //LegalMoveCache in GameFigure --> mit entsprechenden methoden
    //Caclulate all Legal Moves --> ruft bei allen figure update moves map auf (oder vielleicht kann  man sogar bei manchen cachen wenn sie nicht bewegt wurden oder so)
    //isMoveLegal() umstellen nur von der gezougenen figur wird geguckt ob bei geogener figr move im move cache
    //make move bleibt fast gleich nur would be in check bleibt weg
    //eigentlich performance schlechter aber glaube auch für ai sinnvoll und für gui wäre auch sinnvoller


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
        bool wouldBeInCheck(const Move& move, MoveResult moveResult);
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


        CachedThreats simulateUpdateThreatendSquares(GameFigure* capturedFigure, const Move& move);
            void simulateRemoveOldThreats(GameFigure* figure, CachedThreats& cachedThreats);
            void simulateRefreshThreats(GameFigure* figure, CachedThreats& cachedThreats);
        void revertSimulatedThreats(CachedThreats cachedThreats);
            void revertAddedThreats(const CachedThreats cachedThreats, Color color);
            void revertRemovedThreats(CachedThreats cachedThreats, Color color);

        ChangedPieces simulateMove(const Move& move, MoveResult moveresult, std::optional<FigureType> promotedFigureType);        
            ChangedPieces simulateNormalMove(const Move& move);
            ChangedPieces simulateCastelingMove(const Move& move);
            ChangedPieces simulateEnPassantMove(const Move& move);
            ChangedPieces simulatePromotingMove(const Move& move);
        ChangedPieces simulateEditBoard(GameFigure** movedFigure_ptr, GameFigure** capturedFigure_ptr, const Move& move);
        void revertSimulatedMove(const Move& move, const ChangedPieces changedPieces);
            void placeFigureOnBoard(GameFigure* figureToPlace);
    };
}
