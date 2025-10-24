#include "MovementLogic.h"

namespace chess{
    bool KingMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {}
    bool QueenMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {}

    bool RookMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {
        if(isAllowedDirection(Move) && isPathClear(Move, BoardView))
            return true;

        return false;
    }
    bool RookMovment::isAllowedDirection(const Move& Move) const{
        int x_OffSet = Move.m_OffSetPosition.x;
        int y_OffSet = Move.m_OffSetPosition.y;
        if((x_OffSet && !y_OffSet) || (!x_OffSet && y_OffSet))
            return true;

        return false;       
    } 
    bool RookMovment::isPathClear(const Move& Move, const BoardView& BoardView) const{
        int stepX = (Move.m_OffSetPosition.x == 0) ? 0 : (Move.m_OffSetPosition.x > 0 ? 1 : -1);
        int stepY = (Move.m_OffSetPosition.y == 0) ? 0 : (Move.m_OffSetPosition.y > 0 ? 1 : -1);

        int x = Move.m_PiecePosition.x + stepX;
        int y = Move.m_PiecePosition.y + stepY;

        while(x != Move.m_DesiredPosition.x || y != Move.m_DesiredPosition.y){
            if(BoardView.getFigureAt({ x, y}))
                return false;
            x+=stepX;
            y+=stepY;
        }

        return true;
    }

    bool KnightMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {}
    bool BishopMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {}
    bool PawnMovment::isMoveLegal(const Move& Move, const BoardView& BoardView) const {}

}