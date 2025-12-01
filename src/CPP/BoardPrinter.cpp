#include "BoardPrinter.h"

namespace chess{
    FigureType ASCIIPrinter::getPromotionFigure() const{

    }

    void ASCIIPrinter::printBoard(const BoardView& BoardView)const{
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
        std::cout << "-----------------------------------------------" << "\n";
        for(int i = 0; i<boardWidth;i++){
            std::cout << i+1 << " "; 
            for(int j = 0; j<boardWidth;j++){
                std::cout << " | ";
                Position curr_pos(i, j);
                const GameFigure* toprintFigure = BoardView.getFigureAt(curr_pos); 
                if(toprintFigure){
                    //Später ändern wenn FigurePrinter implementiert
                    toprintFigure->printChar();
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << i+1 <<"\n";
            std::cout << "-----------------------------------------------" << "\n";
        }
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
    }
}