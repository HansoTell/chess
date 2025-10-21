#include "GameFigure.h"


namespace chess{

    GameFigure::GameFigure(Color color, char figureChar, Position& pos): m_Color(color), m_FigureChar(figureChar), m_Position(pos){}
    Pawn::Pawn(Color color, Position& pos) : GameFigure(color, 'P', pos){}
    Rook::Rook(Color color, Position& pos) : GameFigure(color, 'R', pos){}
    Bishop::Bishop(Color color, Position& pos) : GameFigure(color, 'B', pos){}
    Horse::Horse(Color color, Position& pos) : GameFigure(color, 'K', pos){}
    Queen::Queen(Color color, Position& pos) : GameFigure(color, 'Q', pos){}
    King::King(Color color, Position& pos) : GameFigure(color, 'K', pos){}

    void GameFigure::printColor() const{
        if(m_Color == Color::WHITE){
            std::cout << 'W';
        }else{
            std::cout << 'B';
        }
    }
    void GameFigure::printChar() const{
        printColor();
        std::cout <<m_FigureChar;
    }
    bool GameFigure::isMoveLegal(const Move& Move, const BoardView& BoardView) const {
        if(isAllowedDirection(Move) && isPathClear(Move, BoardView))
            return true;

        return false;
    }


    bool Rook::isAllowedDirection(const Move& Move) const {
        int x_OffSet = Move.m_OffSetPosition.x;
        int y_OffSet = Move.m_OffSetPosition.y;
        if((x_OffSet && !y_OffSet) || (!x_OffSet && y_OffSet))
            return true;

        return false;        
    }
    bool Rook::isPathClear(const Move& Move, const BoardView& BoardView) const {
        //1d:
        int endXPosToCheck;
        int smaller;
        int bigger;
        if(Move.m_OffSetPosition.x<0){
        
        } 
        for(int i = smaller; i< bigger;i++){
            if(BoardView.getFigureAt( {i, Move.m_PiecePosition.y} ))
                return false;
        }

        return true;
    }

    bool Pawn::isAllowedDirection(const Move& Move) const {}
    bool Pawn::isPathClear(const Move& Move, const BoardView& BoardView) const {}

    bool Bishop::isAllowedDirection(const Move& Move) const {}
    bool Bishop::isPathClear(const Move& Move, const BoardView& BoardView) const {}

    bool Horse::isAllowedDirection(const Move& Move) const {}
    bool Horse::isPathClear(const Move& Move, const BoardView& BoardView) const {}

    bool Queen::isAllowedDirection(const Move& Move) const {}
    bool Queen::isPathClear(const Move& Move, const BoardView& BoardView) const {}

    bool King::isAllowedDirection(const Move& Move) const {}
    bool King::isPathClear(const Move& Move, const BoardView& BoardView) const {}



    std::unique_ptr<GameFigure> GameFigureFactory(const std::string& type, Color Color, Position& pos){
        if(type == "PAWN") return std::make_unique<Pawn>(Color, pos);
        else if(type=="ROOK") return std::make_unique<Rook>(Color, pos);
        else if(type=="BISHOP") return std::make_unique<Bishop>(Color, pos);
        else if(type=="HORSE") return std::make_unique<Horse>(Color, pos);
        else if(type=="QUEEN") return std::make_unique<Queen> (Color, pos);
        else if(type=="KING") return std::make_unique<King>(Color, pos);
        else{
            std::cout << "Kein Valide Figur eingegeben im json dokument" << "\n";
            return nullptr;
        }
    }
}
