#pragma once

#include "Move.h"
#include "GameFigure.h"

#include <vector>
#include <tuple>


namespace chess{

    struct CachedThreats {
        std::vector<Position> addedThreatsWhite;
        std::vector<Position> addedThreatsBlack;

        std::vector<std::tuple<GameFigure*, std::vector<Position>>> removedThreatsWhite;
        std::vector<std::tuple<GameFigure*, std::vector<Position>>> removedThreatsBlack;

        const std::vector<Position>& getAddedThreats(Color color) const { return (color == WHITE) ? addedThreatsWhite : addedThreatsBlack; }
        std::vector<std::tuple<GameFigure*, std::vector<Position>>>& getRemovedThreats(Color color) { return (color == WHITE) ? removedThreatsWhite : removedThreatsBlack; }
    };


    struct ChangedPieces {
        ChangedPieces(GameFigure* movedPiece, std::optional<Position> movedPiecePos, 
                      GameFigure* capturedPiece,
                      GameFigure* castelingRook, std::optional<Position> castelingRookPos) : 
                m_MovedPiece(movedPiece), m_MovedPiecePos(movedPiecePos), 
                m_CapturedPiece(capturedPiece), 
                m_CastelingRook(castelingRook), m_CastelingRookPos(castelingRookPos) {}

        GameFigure* m_MovedPiece = nullptr;
        std::optional<Position> m_MovedPiecePos;
        GameFigure* m_CapturedPiece = nullptr;
        GameFigure* m_CastelingRook = nullptr;
        std::optional<Position> m_CastelingRookPos;
    };
}