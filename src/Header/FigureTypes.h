#pragma once

#include <nlohmann/json.hpp>

namespace chess{

    enum FigureType{
        KING = 0, QUEEN, KNIGHT, BISHOP, ROOK, PAWN
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(FigureType, {
        {FigureType::KING, "KING"},
        {FigureType::QUEEN, "QUEEN"},
        {FigureType::BISHOP, "BISHOP"},
        {FigureType::KNIGHT, "KNIGHT"},
        {FigureType::ROOK, "ROOK"},
        {FigureType::PAWN, "PAWN"},
    })
}
