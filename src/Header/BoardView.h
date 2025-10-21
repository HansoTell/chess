#pragma once

#include <array>
#include "Constants.h"
#include "Move.h"


namespace chess{
    class GameFigure;

    class BoardView
    {
    public:
        BoardView(const std::array<GameFigure*, boardSize>* const boardPtr);
        BoardView(const BoardView& other) = default;
        BoardView(BoardView&& other) = default;

        const GameFigure* getFigureAt(const Position& pos) const;
    private:
        const std::array<GameFigure*, boardSize>* const m_BoardPtr; 

    };
}