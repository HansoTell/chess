#pragma once
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>
#include "Color.h"
#include "Move.h"
#include "FigureTypes.h"


using json = nlohmann::json;
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
        GameState() { m_BlackThreatendSquares.reserve(74); m_WhiteThreatendSquares.reserve(74); }
        void parseGameStateJson(const json& gameConfig);

        bool hasKingMoved(Color color) const { return (color == WHITE) ? m_HasWhiteKingMoved : m_HasBlackKingMoved; }
        bool hasHRookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteHRookMoved : m_HasBlackHRookMoved; }
        bool hasARookMoved(Color color) const { return (color == WHITE) ? m_HasWhiteARookMoved : m_HasBlackARookMoved; }
        bool isKingInCheck(Color color) const;
        void toggleKingInCheck(Color color);
        void updateGameState(const Move& move, std::optional<MoveType> MoveType, FigureType movedFigureType);
        const std::vector<Position>& getThreatendSquares(Color color) const; 
        std::vector<Position>& getThreatendSquares(Color color);
        void removeThreatsFormThreatMap(const std::vector<Position>& threatsToRemove, Color color);
    };
}