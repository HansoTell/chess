#include "AttackTabels.h"


namespace chess{
    namespace AttackTabels{
        std::array<std::vector<Position>, 64> knightAttacks;
        std::array<std::vector<Position>, 64> kingAttacks;
    
        static const std::array<std::pair<int, int>, 8> knightOffsets = 
                            { std::pair(1,2), { -1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 }, { -1, -2 }, { -2, 1 }, { -2, -1 } };

        static const std::array<std::pair<int, int>, 8> kingOffsets = 
                            { std::pair(0, 1), {1, 1}, {1,0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

        template<size_t T>
        void createTabel(std::array<std::vector<Position>, 64>& attackTabel, const std::array<std::pair<int,int>, T>& offsetTabel){
            for(int i = 0; i < attackTabel.size(); i++){
                int x = i / 8;
                int y = i % 8;
                for(auto[dx, dy]& : offsetTabel){
                    int offsetX = x + dx;
                    int offsetY = y + dy;
                    if(offsetX >= 0 && offsetX < 8 && offsetY >= 0 && offsetY < 8)
                        attackTabel[i].emplace_back(x, y); 
                }
            }
        }

        void initAttackTabels(){
            createTabel(knightAttacks, knightOffsets);
            createTabel(kingAttacks, kingOffsets);
        }
    }
}