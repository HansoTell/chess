#pragma once
#include <array>
#include <vector>
#include <Move.h>
#include <utility>

namespace chess{
    namespace AttackTabels{
        extern std::array<std::vector<Position>, 64> knightAttacks;
        extern std::array<std::vector<Position>, 64> kingAttacks;
        void initAttackTabels();
    }
}