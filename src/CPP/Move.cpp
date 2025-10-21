#include "Move.h"

namespace chess
{
  Position::Position(Position&& other) : x(other.x), y(other.y){
    other.x=0;
    other.y=0;
  }  
} 
