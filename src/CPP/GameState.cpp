#include "GameState.h"

namespace chess{
    bool GameState::isKingInCheck(Color color) const {
        if(color == WHITE){
            return m_isWhiteKingInCheck;
        }else{
            return m_isBlackKingInCheck;
        }
    }

    void GameState::toggleKingInCheck(Color color){
        if(color == WHITE){
            m_isWhiteKingInCheck = !m_isWhiteKingInCheck;
        }else{
            m_isBlackKingInCheck = !m_isBlackKingInCheck;
        }
    }

    void GameState::setKingMoved(Color color){
        if(color == WHITE){
            m_HasWhiteKingMoved = true;
        }else{ 
            m_HasBlackKingMoved = true;
        }
    }

    void GameState::setHRookMoved(Color color){
        if(color == WHITE){
            m_HasWhiteHRookMoved = true;
        }else{ 
            m_HasBlackHRookMoved = true;
        }
    }

    void GameState::setARookMoved(Color color){
        if(color == WHITE){
            m_HasWhiteARookMoved= true;
        }else{ 
            m_HasBlackARookMoved= true;
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

    void GameState::updateGameState(const Move& move, std::optional<MoveType> MoveType, FigureType movedFigureType){
        if(MoveType == CASTEL){
            bool isShortCastle = move.getXOffSet() > 0;
            setKingMoved(move.m_PlayerColor);
            if(isShortCastle){
                setHRookMoved(move.m_PlayerColor);
            }else{
                setARookMoved(move.m_PlayerColor);
            }
            return;
        }

        if(movedFigureType == ROOK){
            if(move.m_PiecePosition.x == 0 && (move.m_PiecePosition.y == 0 || move.m_PiecePosition.y == 7))
                setARookMoved(move.m_PlayerColor);
            
            if(move.m_PiecePosition.x == 7 && (move.m_PiecePosition.y == 0 || move.m_PiecePosition.y == 7))
                setHRookMoved(move.m_PlayerColor);
            
            return;
        }

        if(movedFigureType == KING && !hasKingMoved(move.m_PlayerColor)){
            setKingMoved(move.m_PlayerColor);   
            return;
        }
    }

    void GameState::removeThreatsFormThreatMap(const std::vector<Position>& threatsToRemove, Color color){

        std::vector<Position>& threatMap = getThreatendSquares(color);

        for(auto& pos : threatsToRemove){
            auto toRemoveThreat = std::find(threatMap.cbegin(), threatMap.cend(), pos);

            if(toRemoveThreat != threatMap.end()){
                threatMap.erase(toRemoveThreat);
            }
        }
    }

}