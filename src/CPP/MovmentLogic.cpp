#include "MovementLogic.h"

//Casteling is missing
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
        static bool isEnPassant(const Move& move, const BoardView& BoardView){
            const Position isEnPassantPawnPos(move.m_DesiredPosition.x, move.m_DesiredPosition.y -1);
            int EnPassantRow = (move.m_PlayerColor == WHITE) ? 4 : 3;

           return BoardView.getFigureAt( isEnPassantPawnPos ) && 
                    BoardView.getFigureAt(isEnPassantPawnPos)->getFigureType() == PAWN &&
                        EnPassantRow == move.m_PiecePosition.y; 
        }
        static bool isCastle(const Move& move){
            
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


    bool RookMovement::isMoveLegal(const Move& Move, const BoardView& BoardView) const {
        if(MovementLogic::isMoveLegal(Move, BoardView)){
            m_HasMoved = true;
            return true;
        }
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

    bool KingMovement::isMoveLegal(const Move& Move, const BoardView& BoardView) const {
        if(MovementLogic::isMoveLegal(Move, BoardView)){
            m_HasMoved = true;
            return true;
        }
        return false;
    }
    bool KingMovement::isAllowedDirection(const Move& Move) const { 
        if(MovementTypes::isKingMovement(Move))
            return true;

        if(!m_HasMoved && MovementTypes::isCastle(Move))
            return true;
        

        return false; 
    }

    
    bool QueenMovement::isAllowedDirection(const Move& Move) const { return (MovementTypes::isStraigt(Move) || MovementTypes::isDiagonal(Move)); }
    bool QueenMovement::isPathClear(const Move& Move, const BoardView& BoardView) const {
        int stepX = (Move.m_OffSetPosition.x == 0) ? 0 : (Move.m_OffSetPosition.x > 0 ? 1 : -1);
        int stepY = (Move.m_OffSetPosition.y == 0) ? 0 : (Move.m_OffSetPosition.y > 0 ? 1 : -1);

        return MovementTypes::isPathClear(Move, BoardView, stepX, stepY);
    }

    bool PawnMovement::isAllowedDirection(const Move& move) const{

        if(!move.m_OffSetPosition.y)
            return false;

        if((move.m_PlayerColor == WHITE) && (move.m_OffSetPosition.y < 0))
            return false;

        if((move.m_PlayerColor == BLACK) && (move.m_OffSetPosition.y > 0))
            return false;


        if(MovementTypes::isStraigt(move) && abs(move.m_OffSetPosition.y) <= 2){
            int startPosition = (move.m_PlayerColor == WHITE) ? 1 : 7;
            if(abs(move.m_OffSetPosition.y == 2) && (startPosition != move.m_PiecePosition.y))
               return false;
            
            return true;
        }
        
        if(MovementTypes::isDiagonal(move) && abs(move.m_OffSetPosition.y == 1))
            return true;
        
        return false;
    }

    bool PawnMovement::isPathClear(const Move& move, const BoardView& BoardView) const {
        if(MovementTypes::isStraigt(move)){
            int stepX = 0;
            int stepY = (move.m_PlayerColor == WHITE) ? 1 : -1;
            Move moveCopy = move;
            moveCopy.m_DesiredPosition.y += stepY;

            return MovementTypes::isPathClear(moveCopy, BoardView, stepX, stepY);
        }

        if(isDiagonalPathClear(move, BoardView))
            return true;
        
        if(MovementTypes::isEnPassant(move, BoardView))
            return true;
         

        return false;
    }
}