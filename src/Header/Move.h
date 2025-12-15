#pragma once
#include "Color.h"
#include "Constants.h"
#include "MoveType.h"

#include <iostream>

namespace chess{

    struct Position{
        int x, y;
        Position(int x, int y) : x(x), y(y){}
        Position(const Position& other) = default;
        Position(Position&& other);
        Position& operator=(const Position& other) = default;
        bool operator==(const Position& other) const { return this->x == other.x && this->y == other.y; }
        bool operator!=(const Position& other) const { return !((*this) == other); }
        int index() const { return x + y * boardWidth; }
    };

    bool isPositionOutOfBounds(Position pos); 

    std::ostream& operator<<(std::ostream& os, const Position& position);

    struct Move{
        Position m_PiecePosition, m_DesiredPosition;
        Color m_PlayerColor;
        Move(Position piecePosition, Position desiredPosition, Color playerColor) : 
            m_PiecePosition(piecePosition), m_DesiredPosition(desiredPosition), m_PlayerColor(playerColor){}
        Move(const Move& other) = default;

        int getXOffSet() const { return m_DesiredPosition.x - m_PiecePosition.x; }
        int getYOffSet() const { return m_DesiredPosition.y - m_PiecePosition.y; }
        bool isOutOfBounds() const;
    };

    std::ostream& operator<<(std::ostream& os, const Move& move);
}