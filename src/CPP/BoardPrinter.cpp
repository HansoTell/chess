#include "BoardPrinter.h"

namespace chess{
    FigureType ASCIIPrinter::getPromotionFigure() const{

    }

    void ASCIIPrinter::printBoard(const BoardView& BoardView)const{
        printHeader();
        for(int i = 0; i<boardWidth;i++){
            std::cout << i+1 << " "; 
            for(int j = 0; j<boardWidth;j++){
                std::cout << " | ";
                Position curr_pos(i, j);
                const GameFigure* toprintFigure = BoardView.getFigureAt(curr_pos); 
                if(toprintFigure){
                    printFigure(toprintFigure->getFigureType(), toprintFigure->getColor());
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << i+1 <<"\n";
            std::cout << "-----------------------------------------------" << "\n";
        }
        printFooter();
    }


    void ASCIIPrinter::printHeader() const {
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
        std::cout << "-----------------------------------------------" << "\n";
    }

    void ASCIIPrinter::printFooter() const {
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
    }

    void ASCIIPrinter::printFigure(FigureType figureType, Color color) const {
        if(color == WHITE){
            std::cout << 'W';
        }else{
            std::cout << 'B';
        }

        switch (figureType)
        {
        case PAWN:
            std::cout << 'P';
            break;
        case ROOK:
            std::cout << 'R';
            break;
        case BISHOP:
            std::cout << 'B';
            break;
        case KNIGHT:
            std::cout << 'H';
            break;
        case QUEEN:
            std::cout << 'Q';
            break;
        case KING:
            std::cout << 'K';
            break;
        }

    }
}