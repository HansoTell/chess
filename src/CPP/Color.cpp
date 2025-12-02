#include "Color.h"

namespace chess{
    Color opposite(Color color){ return (color == WHITE) ? BLACK : WHITE; }
}