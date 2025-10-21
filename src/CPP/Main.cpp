#include <iostream>
#include "Board.h"

int main(){
    std::string source ="src/utilities/deafult_positions.json"; 
    chess::Board b(source);
    b.printBoard();
    
    return 0;
}