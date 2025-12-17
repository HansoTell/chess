#include <iostream>
#include <memory>
#include <stdlib.h>


#include "Board.h"

bool g_bQuit = false;
chess::Color g_eCurrColor = chess::Color::WHITE;

chess::Position reciveInput( std::string& out ){ 
    std::cin >> out;
    if(out == "q")
        g_bQuit = true;


    char c = out[1];
    return chess::Position( toupper( out[0] ) - 'A', atoi( out.c_str() + (&c - &out[0])) - 1);
}

bool moveAusführen(chess::Board& board){

    std::string input;
    std::cout << "Gebe Position von Figur zu bewegen an in der Form BuchstabeZahl: ";

    chess::Position piecePosition = reciveInput(input);

    std::cout << "Gebe die Ziel Position angeben in der Form BuchstabeZahl: ";
    
    chess::Position desiredPosition = reciveInput(input);

    chess::Move move(piecePosition, desiredPosition, g_eCurrColor);

    return board.makeMove(move);
}


int main(){
    std::string source ="src/utilities/test_check1.json"; 
    chess::Board board(source, std::make_unique<chess::ASCIIPrinter>());
    //Board init methode?

    board.printBoard();

    while( !g_bQuit ){

        if( moveAusführen(board) ){
            board.printBoard();
            //chematisch
            if( board.isCheckmate(chess::opposite(g_eCurrColor)) ){
                g_bQuit = true;
            }

            if( board.isStalemate() ){
                g_bQuit = true;
            }
            g_eCurrColor = chess::opposite(g_eCurrColor);
        }else{
            std::cout << "Ungültiger Move bitte versuche es erneut!" << "\n";
        }

    }
     
    return 0;
}