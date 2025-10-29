#pragma once
#include "Color.h"
#include "Constants.h"
#include "MoveType.h"

namespace chess{

    struct Position{
        int x, y;
        Position(int x, int y) : x(x), y(y){}
        Position(const Position& other) = default;
        Position(Position&& other);
        int index() const { return x + y * boardWidth; }
    };

    struct Move{
        Position m_PiecePosition, m_DesiredPosition, m_OffSetPosition;
        Color m_PlayerColor;
        MoveType m_MoveType;
        Move(Position piecePosition, Position desiredPosition, Color playerColor) : 
            m_PiecePosition(piecePosition), m_DesiredPosition(desiredPosition), m_PlayerColor(playerColor), 
                m_OffSetPosition(m_DesiredPosition.x - m_PiecePosition.x, m_DesiredPosition.y - m_PiecePosition.y), m_MoveType(NORMAL){}
        Move(const Move& other) = default;
    };
}