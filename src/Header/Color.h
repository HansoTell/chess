#pragma once


#include <iostream>

namespace chess{
    enum Color{
        WHITE = 0, BLACK
    };

    Color opposite(Color color);

    std::ostream& operator<<(std::ostream& os, Color color);
}