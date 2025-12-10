#include <iostream>
#include <memory>


#include "Board.h"


int main(){
    std::string source ="src/utilities/deafult_positions.json"; 
    chess::Board b(source, std::make_unique<chess::ASCIIPrinter>());

    bool bQuit = false;
    chess::Color currColor = chess::Color::WHITE;

    while( !bQuit ){
        b.printBoard();

        currColor = chess::opposite(currColor);
        std::string input;
        std::cout << "Gebe Position von Figur zu bewegen an: ";
        std::cin >> input;
        if(input == "q")
            bQuit = true;

        //casten so dass move erstellt wird


    }
     
    return 0;
}