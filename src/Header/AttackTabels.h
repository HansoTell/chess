#pragma once
#include <array>
#include <vector>
#include <Move.h>
#include <utility>

namespace chess{
    namespace AttackTabels{
        extern std::array<std::vector<Position>, 64> knightAttacks;
        extern std::array<std::vector<Position>, 64> kingAttacks;
        extern std::array<std::pair<int, int>, 4> StraightSteps; 
        extern std::array<std::pair<int, int>, 4> DiagonalSteps; 
        void initAttackTabels();
    }
}