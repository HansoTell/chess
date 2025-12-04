#pragma once

#include "Move.h"
#include "GameFigure.h"

#include <vector>
#include <tuple>


namespace chess{

    struct CachedThreats {
        std::vector<Position> addedThreatsWhite;
        std::vector<Position> addedThreatsBlack;

        std::vector<std::tuple<const GameFigure*, std::vector<Position>>> removedThreatsWhite;
        std::vector<std::tuple<const GameFigure*, std::vector<Position>>> removedThreatsBlack;
    };


    struct ChangedPieces {
        ChangedPieces(GameFigure* movedPiece, GameFigure* capturedPiece, GameFigure* castelingRook) : 
                m_MovedPiece(movedPiece), m_CapturedPiece(capturedPiece), m_CastelingRook(castelingRook) {}

        GameFigure* m_MovedPiece;
        GameFigure* m_CapturedPiece;
        GameFigure* m_CastelingRook;
    };
}