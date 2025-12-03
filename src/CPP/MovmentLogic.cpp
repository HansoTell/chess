#include "MovementLogic.h"
#include "GameFigure.h"

namespace chess{

    namespace MoveChecks{
        static bool isStraigt(const Move& Move){
            int x_OffSet = Move.getXOffSet();
            int y_OffSet = Move.getYOffSet();
            if((x_OffSet && !y_OffSet) || (!x_OffSet && y_OffSet))
                return true;

            return false;       
        }
        static bool isDiagonal(const Move& Move){
            return abs(Move.getXOffSet()) == abs(Move.getYOffSet());
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
            int movementDirection = (move.m_PlayerColor == WHITE) ? 1 : -1;
            const Position isEnPassantPawnPos(move.m_DesiredPosition.x, move.m_DesiredPosition.y - movementDirection);
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
        static bool isCastle(const Move& move, const BoardView& BoardView, const GameState& GameState){
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
            

            int checkFrom = (isShortCastle == 1) ? move.m_PiecePosition.x : move.m_DesiredPosition.x;
            int checkTo = (isShortCastle == 1) ? move.m_DesiredPosition.x : move.m_PiecePosition.x ;
            const std::vector<Position>& enemyThreats = GameState.getThreatendSquares(opposite(move.m_PlayerColor));
            for(int i = checkFrom; i <= checkTo; i++){
                if(std::find(enemyThreats.begin(), enemyThreats.end(), Position(i, move.m_PiecePosition.y)) != enemyThreats.end()){
                    return false;
                }
            }
            
            return true;
        }
        static bool isPromoting(const Move& move){
            int promotingRow = (move.m_PlayerColor == WHITE) ? 7 : 0;

            return move.m_DesiredPosition.y == promotingRow;
        }
    }

    namespace ThreatTypes{
        template<typename F, size_t T>
        void getSlidingThreats(const BoardView& BoardView, const Position& figPos, std::array<std::pair<int, int> , T> steps, F callback){
            for(auto& [dx, dy] : steps){
                Position newPos(figPos.x+dx, figPos.y+dy);
                while(newPos.x >=0 && newPos.x <8 && newPos.y >=0 && newPos.y<8 && !BoardView.getFigureAt(newPos)){
                    callback(newPos);
                    newPos.x+=dx;
                    newPos.y+=dy;
                }
                if(newPos.x >=0 && newPos.x <8 && newPos.y >=0 && newPos.y<8)
                    callback(newPos);
            } 
        }
    }

    MoveResult RookMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const {
        if(MoveChecks::isStraigt(move)){

            int stepX = (move.getXOffSet() == 0) ? 0 : (move.getXOffSet() > 0 ? 1 : -1);
            int stepY = (move.getYOffSet() == 0) ? 0 : (move.getYOffSet() > 0 ? 1 : -1);

            return { MoveChecks::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }
    MoveResult BishopMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const {
        if(MoveChecks::isDiagonal(move)){

            int stepX = (move.getXOffSet() > 0) ? 1 : -1;
            int stepY = (move.getYOffSet() > 0) ? 1 : -1;

            return { MoveChecks::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }
    MoveResult KnightMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const { return { MoveChecks::isKnightMovement(move), NORMAL }; }
    MoveResult KingMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const { 

        if(MoveChecks::isKingMovement(move))
            return { true, NORMAL };

        if(MoveChecks::isCastle(move, BoardView, GameState))
            return {true, CASTEL};
        

        return false; 
    }
    MoveResult QueenMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const { 
        if(MoveChecks::isStraigt(move) || MoveChecks::isDiagonal(move)){

            int stepX = (move.getXOffSet() == 0) ? 0 : (move.getXOffSet() > 0 ? 1 : -1);
            int stepY = (move.getYOffSet() == 0) ? 0 : (move.getYOffSet() > 0 ? 1 : -1);

            return { MoveChecks::isPathClear(move, BoardView, stepX, stepY), NORMAL };
        }

        return false;
    }
    MoveResult PawnMovement::isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const { 
        if(!move.getYOffSet())
            return false;

        if((move.m_PlayerColor == WHITE) && (move.getYOffSet() < 0))
            return false;

        if((move.m_PlayerColor == BLACK) && (move.getYOffSet() > 0))
            return false;


        MoveType moveType = (MoveChecks::isPromoting(move)) ? PROMOTING : NORMAL; 

        if(MoveChecks::isStraigt(move) && abs(move.getYOffSet()) <= 2){
            int startPosition = (move.m_PlayerColor == WHITE) ? 1 : 7;
            if(abs(move.getYOffSet() == 2) && (startPosition != move.m_PiecePosition.y))
               return false;
            
            
            int stepX = 0;
            int stepY = (move.m_PlayerColor == WHITE) ? 1 : -1;
            Move moveCopy = move;
            moveCopy.m_DesiredPosition.y += stepY;

            return { MoveChecks::isPathClear(moveCopy, BoardView, stepX, stepY), moveType};
        }
        if(MoveChecks::isDiagonal(move) && abs(move.getYOffSet() == 1)){
            if(BoardView.getFigureAt(move.m_DesiredPosition))
                return {isDiagonalPathClear(move, BoardView), moveType};
            
            
            if(MoveChecks::isEnPassant(move, BoardView))
                return {true, EN_PASSANT };
        }

        return false;
    }


    std::vector<Position> RookMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        std::vector<Position> threats;
        threats.reserve(14);
        ThreatTypes::getSlidingThreats(BoardView, figPos, AttackTabels::StraightSteps, [&](Position threatPos){ threats.emplace_back(threatPos); });
        return threats;
    }
    std::vector<Position> BishopMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        std::vector<Position> threats;
        threats.reserve(14);
        ThreatTypes::getSlidingThreats(BoardView, figPos, AttackTabels::DiagonalSteps, [&](Position threatPos){ threats.emplace_back(threatPos); });
        return threats;
    }
    std::vector<Position> KnightMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        std::vector<Position>& threatendSquares = AttackTabels::knightAttacks[figPos.index()];
        return threatendSquares;
    }
    std::vector<Position> KingMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        std::vector<Position>& threatendSquares = AttackTabels::kingAttacks[figPos.index()];
        return threatendSquares;
    }
    std::vector<Position> QueenMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        std::vector<Position> threats;
        threats.reserve(27);
        ThreatTypes::getSlidingThreats(BoardView, figPos, AttackTabels::StraightSteps, [&](Position threatPos){ threats.emplace_back(threatPos); });
        ThreatTypes::getSlidingThreats(BoardView, figPos, AttackTabels::DiagonalSteps, [&](Position threatPos){ threats.emplace_back(threatPos); });
        return threats;
    }
    std::vector<Position> PawnMovement::getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const{
        int direction = (color == WHITE) ? 1 : -1;
        std::vector<Position> threats;
        threats.reserve(4);

        threats.emplace_back(figPos.x+1, figPos.y+direction);
        threats.emplace_back(figPos.x-1, figPos.y+direction);

        int EnPassantRow = (color == WHITE) ? 4 : 3;
        
        if(EnPassantRow == figPos.y){
            Position potentialPawnPos1(figPos.x +1, figPos.y);
            Position potentialPawnPos2(figPos.x -1, figPos.y);
            const GameFigure* potentialPawn1 = BoardView.getFigureAt(potentialPawnPos1);
            const GameFigure* potentialPawn2 = BoardView.getFigureAt(potentialPawnPos2);
            if(potentialPawn1 && potentialPawn1->getFigureType() == PAWN){
                threats.emplace_back(potentialPawnPos1);
            }

            if(potentialPawn2 && potentialPawn2->getFigureType() == PAWN){
                threats.emplace_back(potentialPawnPos2);
            }
        }
        return threats;
    }
}