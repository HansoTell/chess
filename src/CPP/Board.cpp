#include "Board.h"


namespace chess{
    Board::Board(const std::string& file) : m_BoardPositions{}, m_BoardView(&m_BoardPositions) {
        m_Figures.reserve(32 * sizeof(GameFigure));
        boardinit(file);
    }

    json Board::parseJson(const std::string& file){
        std::ifstream Jfile(file);
        if(!Jfile){
            //Exception handeling
            std::cout << "File konnte nicht eingelesen werden\n";
        }

        json j;
        Jfile >> j;
        return j;
    }

    void Board::boardinit(const std::string& file){
        json boardPositionJson = parseJson(file);

        fillBoard(boardPositionJson);
    }

    void Board::fillBoard(const json& jsonMap){

        auto& figure_position_map= jsonMap["figure_positions"];
        
        for(auto&[figure_Type, data_Figures] : figure_position_map.items()){
            for(auto& data_figure : data_Figures){
                int posX = data_figure[1].get<int>();
                int posY = data_figure[2].get<int>();
                Position piecePosition(posX, posY);
                
                int colorInt = data_figure[0].get<int>();
                Color pieceColor = static_cast<Color>(colorInt);
                GameFigure figure = GameFigureFactory(figure_Type, pieceColor, piecePosition);

                m_Figures.push_back(std::move(figure));
                m_BoardPositions[posX + posY*boardWidth] = &m_Figures.back();
            }
        }
    }

    void Board::printBoard() const {
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
        std::cout << "-----------------------------------------------" << "\n";
        for(int i = 0; i<boardWidth;i++){
            std::cout << i+1 << " "; 
            for(int j = 0; j<boardWidth;j++){
                std::cout << " | ";
                GameFigure* toprintFigure = m_BoardPositions[i+j*boardWidth];
                if(toprintFigure){
                    m_BoardPositions[i+j*boardWidth]->printChar();
                }else{
                    std::cout << "  ";
                }
            }
            std::cout << " |  " << i+1 <<"\n";
            std::cout << "-----------------------------------------------" << "\n";
        }
        std::cout << "   | A  | B  | C  | D  | E  | F  | G  | H  |" << "\n";
    }


    bool Board::makeMove(const Move& move){

        MoveResult moveResult = isMoveLegal(move);

        if(!moveResult.m_IsMoveLegal){
            return false;
        }

        //theoretisch also wenn halt movetype nicht gesetzt ist was mache wir dann? Oder kann das einfach net vorkommen

        executeMove(move);

        //brauchen check ob figur verwandelt werden kann(bauer auf 8tem rang)

        return false;
    }

    void Board::executeMove(const Move& Move){
        
    }

    MoveResult Board::isMoveLegal(const Move& move) const{
        //überhaupt gucken das in der ausgangsposition auch schön eine figur ist
        //auch noch problem bei kson datei gamestate beim erstellen deafult afu nicht moved was falsch ist wenn rooks weg sind oder so
        //checken ob Move out of bounds
        //checken dass move nicht stehend:
        MoveResult moveResult = m_BoardPositions[move.m_PiecePosition.index()]->isMoveLegal(move, m_BoardView, m_GameState);
        if(!moveResult.m_IsMoveLegal) 
            return false;

        //Brauchen check ob eigener oder anderer könig im schach ist sollte eigener im schach sein falscher zug, sollte anderer im schach sein --> gamestate hscach auf true setzten
        return moveResult;
    }
}