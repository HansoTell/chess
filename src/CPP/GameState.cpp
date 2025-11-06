#include "GameState.h"

namespace chess{
    std::vector<Position>& GameState::getThreatendSquares(Color color) {
        if(color == WHITE){
            return m_WhiteThreatendSquares; 
        }else{
            return m_BlackThreatendSquares; 
        }
    }
}