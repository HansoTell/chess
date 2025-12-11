#include "Move.h"

namespace chess
{
  Position::Position(Position&& other) : x(other.x), y(other.y){
    other.x=0;
    other.y=0;
  }  

  std::ostream& operator<<(std::ostream& os, const Position& position){
    os << "X: " << position.x << " Y: " << position.y;
    return os;
  }

  bool Move::isOutOfBounds() const {
    if(m_PiecePosition.x > boardWidth-1 || m_PiecePosition.x < 0)
      return true;
    if(m_PiecePosition.y > boardWidth-1 || m_PiecePosition.y < 0)
      return true;
    
    if(m_DesiredPosition.x > boardWidth-1 || m_DesiredPosition.x < 0)
      return true;

    if(m_DesiredPosition.y > boardWidth-1 || m_DesiredPosition.y < 0)
      return true;

    return false;
  }


    std::ostream& operator<<(std::ostream& os, const Move& move){
      os << "PiecePos: " << move.m_PiecePosition << " DesiredPos: " << move.m_DesiredPosition << " Color: " << move.m_PlayerColor;
      return os;
    }
} 
