#include "Move.h"

namespace chess
{
  bool isPositionOutOfBounds(Position pos) { return pos.x > boardWidth-1 || pos.x < 0 || pos.y > boardWidth-1 || pos.y < 0; }

  Position::Position(Position&& other) : x(other.x), y(other.y){
    other.x=0;
    other.y=0;
  }  

  std::ostream& operator<<(std::ostream& os, const Position& position){
    os << "X: " << position.x << " Y: " << position.y;
    return os;
  }

  bool Move::isOutOfBounds() const {
    return isPositionOutOfBounds(m_PiecePosition) || isPositionOutOfBounds(m_DesiredPosition);

  }


    std::ostream& operator<<(std::ostream& os, const Move& move){
      os << "PiecePos: " << move.m_PiecePosition << " DesiredPos: " << move.m_DesiredPosition << " Color: " << move.m_PlayerColor;
      return os;
    }
} 
