#pragma once
#include <vector>
#include "Color.h"
#include "Move.h"

namespace chess{

    class GameState
    {
    private:
        std::vector<Position> m_BlackThreatendSquares;
        std::vector<Position> m_WhiteThreatendSquares;

        bool m_HasWhiteKingMoved = false;
        bool m_isWhiteKingInCheck = false;
        bool m_HasWhiteARookMoved = false; 
        bool m_HasWhiteHRookMoved = false; 
        bool m_HasBlackARookMoved = false; 
        bool m_HasBlackHRookMoved = false; 
        bool m_HasBlackKingMoved = false;
        bool m_isBlackKingInCheck = false;
    private:


    public:
        GameState() { m_BlackThreatendSquares.reserve(64 * sizeof(Position)); m_WhiteThreatendSquares.reserve(64*sizeof(Position)); }

        bool hasKingMoved(Color color) const { return (color == WHITE) ? m_HasWhiteKingMoved : m_HasBlackKingMoved; }
        bool hasHRookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteHRookMoved : m_HasBlackHRookMoved; }
        bool hasARookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteARookMoved : m_HasBlackARookMoved; }
        std::vector<Position>& getThreatendSquares(Color color); 
    };
}