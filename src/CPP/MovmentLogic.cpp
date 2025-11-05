#include "MovementLogic.h"
#include "GameFigure.h"


namespace chess{

    namespace MovementTypes{
        static bool isStraigt(const Move& Move){
            int x_OffSet = Move.getXOffSet();
            int y_OffSet = Move.getYOffSet();
            if((x_OffSet && !y_OffSet) || (!x_OffSet && y_OffSet))
                return true;

            return false;       
        }
        static bool isDiagonal(const Move& Move){
            if(abs(Move.getXOffSet())== abs(Move.getYOffSet()))
                return true;
            return false;
        }
        static bool isKnightMovement(const Move& Move){
            int abs_DealtaX = abs(Move.getXOffSet());
            int abs_DealtaY = abs(Move.getYOffSet());
            if((abs_DealtaX == 1 && abs_DealtaY == 2) || (abs_DealtaX == 2 && abs_DealtaY == 1))
                return true;    
            return false;
        }
        static bool isKingMovement(const Move& Move){
            if((abs(Move.getXOffSet()) <= 1) && (abs(Move.getYOffSet() <= 1)))
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
        static bool isCastle(const Move& move, const BoardView& BoardView, GameState GameState){
            if(move.getYOffSet() !=0 || abs(move.getXOffSet()) != 2)
                return false;


            bool isShortCastle = move.getXOffSet() < 0;
            int stepX = (isShortCastle) ? 1 : -1;
            int stepY = 0;
            Move moveCopy = move;
            
            moveCopy.m_DesiredPosition = isShortCastle ?  Position(move.m_DesiredPosition.x + 1, move.m_DesiredPosition.y) : Position(move.m_DesiredPosition.x - 2, move.m_DesiredPosition.y);
            if(!isPathClear(moveCopy, BoardView, stepX, stepY))
               return false;

               
            if(isShortCastle == 1 && (GameState.hasKingMoved(move.m_PlayerColor) || GameState.hasHRookMoved(move.m_PlayerColor)))
                return false;
            
            if(isShortCastle == -1 && (GameState.hasKingMoved(move.m_PlayerColor) || GameState.hasARookMoved(move.m_PlayerColor)))
                return false; 
            


            //einzige bedingun die fehljt sind die bedrohten felder
            
            return true;
        }
        static bool isPromoting(const Move& move){
            int promotingRow = (move.m_PlayerColor == WHITE) ? 7 : 0;

            return move.m_DesiredPosition.y == promotingRow;
        }
    }


    MoveResult RookMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const {
        if(MovementTypes::isStraigt(move)){

            int stepX = (move.getXOffSet() == 0) ? 0 : (move.getXOffSet() > 0 ? 1 : -1);
            int stepY = (move.getYOffSet() == 0) ? 0 : (move.getYOffSet() > 0 ? 1 : -1);

            return { MovementTypes::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }

    MoveResult BishopMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const {
        if(MovementTypes::isDiagonal(move)){

            int stepX = (move.getXOffSet() > 0) ? 1 : -1;
            int stepY = (move.getYOffSet() > 0) ? 1 : -1;

            return { MovementTypes::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }

    MoveResult KnightMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const { return { MovementTypes::isKnightMovement(move), NORMAL }; }

    MoveResult KingMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const { 

        if(MovementTypes::isKingMovement(move))
            return { true, NORMAL };

        if(MovementTypes::isCastle(move, BoardView, GameState))
            return {true, CASTEL};
        

        return false; 
    }

    MoveResult QueenMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const { 
        if(MovementTypes::isStraigt(move) || MovementTypes::isDiagonal(move)){

            int stepX = (move.getXOffSet() == 0) ? 0 : (move.getXOffSet() > 0 ? 1 : -1);
            int stepY = (move.getYOffSet() == 0) ? 0 : (move.getYOffSet() > 0 ? 1 : -1);

            return { MovementTypes::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }

    MoveResult PawnMovement::isMoveLegal(const Move& move, const BoardView& BoardView, GameState GameState) const { 
        //verwandlung fehljt noch
        if(!move.getYOffSet())
            return false;

        if((move.m_PlayerColor == WHITE) && (move.getYOffSet() < 0))
            return false;

        if((move.m_PlayerColor == BLACK) && (move.getYOffSet() > 0))
            return false;


        MoveType moveType = (MovementTypes::isPromoting(move)) ? PROMOTING : NORMAL; 

        if(MovementTypes::isStraigt(move) && abs(move.getYOffSet()) <= 2){
            int startPosition = (move.m_PlayerColor == WHITE) ? 1 : 7;
            if(abs(move.getYOffSet() == 2) && (startPosition != move.m_PiecePosition.y))
               return false;
            
            
            int stepX = 0;
            int stepY = (move.m_PlayerColor == WHITE) ? 1 : -1;
            Move moveCopy = move;
            moveCopy.m_DesiredPosition.y += stepY;

            return { MovementTypes::isPathClear(moveCopy, BoardView, stepX, stepY), moveType};
        }
        
        if(MovementTypes::isDiagonal(move) && abs(move.getYOffSet() == 1))
            return {isDiagonalPathClear(move, BoardView), moveType};
        
        if(MovementTypes::isEnPassant(move, BoardView))
            return {true, EN_PASSANT };

        return false;
    }
}