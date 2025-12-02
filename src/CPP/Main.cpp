#include <iostream>
#include <memory>


#include "Board.h"

int main(){
    std::string source ="src/utilities/deafult_positions.json"; 
    chess::Board b(source, std::make_unique<chess::ASCIIPrinter>());
    b.printBoard();
     
    return 0;
}