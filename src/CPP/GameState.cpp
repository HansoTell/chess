#include "GameState.h"

namespace chess{

    void GameState::toggleKingInCheck(Color color){
        if(color == WHITE){
            m_isWhiteKingInCheck = !m_isWhiteKingInCheck;
        }else{
            m_isBlackKingInCheck = !m_isBlackKingInCheck;
        }
    }

    const std::vector<Position>& GameState::getThreatendSquares(Color color) const {
        if(color == WHITE){
            return m_WhiteThreatendSquares; 
        }else{
            return m_BlackThreatendSquares; 
        }
    }

    std::vector<Position>& GameState::getThreatendSquares(Color color){
        if(color == WHITE){
            return m_WhiteThreatendSquares; 
        }else{
            return m_BlackThreatendSquares; 
        }
    }
}