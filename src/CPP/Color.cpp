#include "Color.h"

namespace chess{


    std::ostream& operator<<(std::ostream& os, Color color){
        if(color==WHITE){
            os << "WHITE";
        }else{
            os << "BLACK";
        }

        return os;
    }

    Color opposite(Color color){ return (color == WHITE) ? BLACK : WHITE; }
}