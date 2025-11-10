#include "GameState.h"

namespace chess{
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