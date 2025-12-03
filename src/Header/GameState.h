#pragma once
#include <vector>
#include <optional>
#include "Color.h"
#include "Move.h"
#include "FigureTypes.h"

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
        void setKingMoved(Color color);
        void setHRookMoved(Color color);
        void setARookMoved(Color color);

    public:
        GameState() { m_BlackThreatendSquares.reserve(64); m_WhiteThreatendSquares.reserve(64); }

        bool hasKingMoved(Color color) const { return (color == WHITE) ? m_HasWhiteKingMoved : m_HasBlackKingMoved; }
        bool hasHRookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteHRookMoved : m_HasBlackHRookMoved; }
        bool hasARookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteARookMoved : m_HasBlackARookMoved; }
        bool isKingInCheck(Color color) const;
        void toggleKingInCheck(Color color);
        void updateGameState(const Move& move, std::optional<MoveType> MoveType, FigureType movedFigureType);
        const std::vector<Position>& getThreatendSquares(Color color) const; 
        std::vector<Position>& getThreatendSquares(Color color);
    };
}