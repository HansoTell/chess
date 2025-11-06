#pragma once
#include <array>
#include <vector>
#include <Move.h>
#include <utility>

namespace chess{
    namespace AttackTabels{
        extern std::array<std::vector<Position>, 64> knightAttacks;
        extern std::array<std::vector<Position>, 64> kingAttacks;
        std::array<std::pair<int, int>, 4> StraightSteps = {std::pair(1,0), {-1,0}, {0,1}, {0,-1}};
        std::array<std::pair<int, int>, 4> DiagonalSteps = {std::pair(1,1), {1,-1}, {-1,-1}, {-1,1}};
        void initAttackTabels();
    }
}