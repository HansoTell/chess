#include "MovementLogic.h"

namespace chess{

    namespace MovementTypes{
        static bool isStraigt(const Move& Move){
            int x_OffSet = Move.m_OffSetPosition.x;
            int y_OffSet = Move.m_OffSetPosition.y;
            if((x_OffSet && !y_OffSet) || (!x_OffSet && y_OffSet))
                return true;

            return false;       
        }
        static bool isDiagonal(const Move& Move){
            if(abs(Move.m_OffSetPosition.x)== abs(Move.m_OffSetPosition.y))
                return true;
            return false;
        }
        static bool isKnightMovement(const Move& Move){
            int abs_DealtaX = abs(Move.m_OffSetPosition.x);
            int abs_DealtaY = abs(Move.m_OffSetPosition.y);
            if((abs_DealtaX == 1 && abs_DealtaY == 2) || (abs_DealtaX == 2 && abs_DealtaY == 1))
                return true;    
            return false;
        }
        static bool isKingMovement(const Move& Move){
            if((abs(Move.m_OffSetPosition.x) <= 1) && (abs(Move.m_OffSetPosition.y <= 1)))
                return true;
            return false; 
        }
        static bool isEnPassant(const Move& Move){

        }
        static bool isPathClear(const Move& Move, const BoardView& BoardView, int stepX, int stepY){
            int x = Move.m_PiecePosition.x + stepX;
            int y = Move.m_PiecePosition.y + stepY;

            while(x != Move.m_DesiredPosition.x || y != Move.m_DesiredPosition.y){
                if(BoardView.getFigureAt({ x, y }))
                    return false;
                x+=stepX;
                y+=stepY;
            }

            return true;
        }
    }

    bool MovementLogic::isMoveLegal(const Move& Move, const BoardView& BoardView) const{
        if(isAllowedDirection(Move) && isPathClear(Move, BoardView))
            return true;

        return false;
    }

    bool RookMovement::isAllowedDirection(const Move& Move) const{ return MovementTypes::isStraigt(Move); } 
    bool RookMovement::isPathClear(const Move& Move, const BoardView& BoardView) const{
        int stepX = (Move.m_OffSetPosition.x == 0) ? 0 : (Move.m_OffSetPosition.x > 0 ? 1 : -1);
        int stepY = (Move.m_OffSetPosition.y == 0) ? 0 : (Move.m_OffSetPosition.y > 0 ? 1 : -1);

        return MovementTypes::isPathClear(Move, BoardView, stepX, stepY);
    }

    bool BishopMovement::isAllowedDirection(const Move& Move) const { return MovementTypes::isDiagonal(Move); }
    bool BishopMovement::isPathClear(const Move& Move, const BoardView& BoardView) const {
        int stepX = (Move.m_OffSetPosition.x > 0) ? 1 : -1;
        int stepY = (Move.m_OffSetPosition.y > 0) ? 1 : -1;

        return MovementTypes::isPathClear(Move, BoardView, stepX, stepY);
    }

    bool KnightMovement::isAllowedDirection(const Move& Move) const { return MovementTypes::isKnightMovement(Move); }

    bool KingMovement::isAllowedDirection(const Move& Move) const { return MovementTypes::isKingMovement(Move); }
    
    bool QueenMovement::isAllowedDirection(const Move& Move) const { return (MovementTypes::isStraigt(Move) || MovementTypes::isDiagonal(Move)); }
    bool QueenMovement::isPathClear(const Move& Move, const BoardView& BoardView) const {
        int stepX = (Move.m_OffSetPosition.x == 0) ? 0 : (Move.m_OffSetPosition.x > 0 ? 1 : -1);
        int stepY = (Move.m_OffSetPosition.y == 0) ? 0 : (Move.m_OffSetPosition.y > 0 ? 1 : -1);

        return MovementTypes::isPathClear(Move, BoardView, stepX, stepY);
    }

    bool PawnMovement::isMoveLegal(const Move& Move, const BoardView& BoardView) const {
        //hat sich nicht in y richtung bewegt
        if(Move.m_OffSetPosition.y)
            return false;

        if((Move.m_PlayerColor == WHITE) && (Move.m_OffSetPosition.y < 0))
            return false;

        if((Move.m_PlayerColor == BLACK) && (Move.m_OffSetPosition.y > 0))
            return false;
        
        if(MovementTypes::isStraigt(Move)){
            
            //kann er 2 schritte gehen
            int startPosition = (Move.m_PlayerColor == WHITE) ? 1 : 7;
            if(startPosition == Move.m_PiecePosition.y){

            }


        }else if(MovementTypes::isDiagonal(Move)){
            if(!MovementTypes::isKingMovement(Move))
                return false;
            
            if(!BoardView.getFigureAt(Move.m_DesiredPosition))
                return false;
            
            return true;
                
        }else if(MovementTypes::isEnPassant(Move)){
            
        }else{
            return false;
        }
    }
    bool PawnMovement::isAllowedDirection(const Move& Move) const{
        //anfangs doppel step --> sonst länge checken
        //enpassant
        //path clear, aber so dass letzte gechekt wird, wenn gerade aus geht


        //--> wissen die figur geht in die richtige richtung
        //--> diagonal sollte er richtig bestimmen können


        int direction = (Move.m_PlayerColor == WHITE) ? 1 : -1;
        int startPosition = (Move.m_PlayerColor == WHITE) ? 1 : 7;

        
    }

    bool PawnMovement::isPathClear(const Move& Move, const BoardView& BoardView) const {

    }
}