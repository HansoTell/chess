#pragma once

namespace chess{
    enum Color{
        WHITE = 0, BLACK
    };


    Color opposite(Color color){ return (color == WHITE) ? BLACK : WHITE; }
}