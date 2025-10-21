#include "BoardView.h"

namespace chess{
    BoardView::BoardView(const std::array<GameFigure*, boardSize>* const boardPtr) : m_BoardPtr(boardPtr){}

    const GameFigure*  BoardView::getFigureAt(const Position& pos) const{ return (*m_BoardPtr)[pos.index()]; }
}