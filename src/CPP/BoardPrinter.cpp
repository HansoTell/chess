#include "BoardPrinter.h"

namespace chess{
    FigureType ASCIIPrinter::getPromotionFigure() const{
        std::cout << "Enter Figure you want to promote to: " << "\n";
        std::cout << "QUEEN" << "\n";
        std::cout << "ROOK" << "\n";
        std::cout << "KNIGHT" << "\n";
        std::cout << "BISHOP" << "\n";
        
        while(true){

            std::string figureType_str;
            std::cin >> figureType_str;

            if(figureType_str == "QUEEN"){
                return QUEEN;
            } else if( figureType_str == "ROOK" ){
                return ROOK;
            } else if(  figureType_str == "KNIGHT" ){
                return KNIGHT;
            }else if( figureType_str == "BISHOP" ){
                return BISHOP;
            }
        }
    }

    void ASCIIPrinter::printThreats(const GameState& gameState, Color color) const{
        printHeader();
        for(int y = 0; y<boardWidth; y++){
            std::cout << y+1 << " ";
            for(int x = 0; x<boardWidth;x++){
                std::cout << " | ";
                Position curr_pos(x,y);
                auto& threatMap = gameState.getThreatendSquares(color);
                if(std::find(threatMap.begin(), threatMap.end(), curr_pos) != threatMap.end()){
                    std::cout << "XX";
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << y+1 <<"\n";
            std::cout << "-----------------------------------------------" << "\n";
        }
        printFooter();
    }

    void ASCIIPrinter::printBoard(const BoardView& BoardView)const{
        printHeader();
        for(int y = 0; y<boardWidth; y++){
            std::cout << y+1 << " "; 
            for(int x = 0; x<boardWidth;x++){
                std::cout << " | ";
                Position curr_pos(x, y);
                const GameFigure* toprintFigure = BoardView.getFigureAt(curr_pos); 
                if(toprintFigure){
                    printFigure(toprintFigure->getFigureType(), toprintFigure->getColor());
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << y+1 <<"\n";
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