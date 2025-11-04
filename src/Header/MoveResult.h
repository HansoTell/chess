#pragma once

#include <optional>
#include "MoveType.h"

namespace chess{
    struct MoveResult
    {
        bool m_IsMoveLegal;
        std::optional<MoveType> m_MoveType;      
        
        MoveResult(bool isMoveLegal) : m_IsMoveLegal(isMoveLegal){}
        MoveResult(bool isMoveLegal, MoveType MoveType) : m_IsMoveLegal(isMoveLegal), m_MoveType(MoveType){}
    };
}