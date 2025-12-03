#pragma once

#include "Move.h"
#include "GameFigure.h"

#include <vector>
#include <tuple>


namespace chess{

    struct CachedThreats {
        std::vector<Position> addedThreatsWhite;
        std::vector<Position> addedThreatsBlack;

        std::vector<std::tuple<const GameFigure*, std::vector<Position>>> removedThreatsWhite;
        std::vector<std::tuple<const GameFigure*, std::vector<Position>>> removedThreatsBlack;
    };
}