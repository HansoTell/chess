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
        ChangedPieces(GameFigure* movedPiece, GameFigure* capturedPiece, GameFigure* castelingRook) : 
                m_MovedPiece(movedPiece), m_CapturedPiece(capturedPiece), m_CastelingRook(castelingRook) {}

        GameFigure* m_MovedPiece = nullptr;
        GameFigure* m_CapturedPiece = nullptr;
        GameFigure* m_CastelingRook = nullptr;
    };
}