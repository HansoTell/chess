#include "GameFigure.h"


namespace chess{

    GameFigure::GameFigure(Color color, char figureChar, Position& pos, std::unique_ptr<MovementLogic> MovementLogic): m_Color(color), m_FigureChar(figureChar), m_Position(pos), m_MovmentLogic(std::move(MovementLogic)){}
    GameFigure::GameFigure(const GameFigure& other) : m_Color(other.m_Color), m_Position(other.m_Position), m_FigureChar(other.m_FigureChar), m_MovmentLogic(other.m_MovmentLogic ? other.m_MovmentLogic->clone() : nullptr){}
    GameFigure::GameFigure(GameFigure&& other) : m_Color(other.m_Color), m_Position(other.m_Position), m_FigureChar(other.m_FigureChar), m_MovmentLogic(std::move(other.m_MovmentLogic)){
        other.m_Color = WHITE;
        other.m_Position.x = 0;
        other.m_Position.y = 0;
        other.m_FigureChar = 'F';
    }

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



    GameFigure GameFigureFactory(const std::string& type, Color Color, Position& pos){
        if(type == "PAWN") return GameFigure(Color, 'P', pos, std::make_unique<PawnMovment>());
        else if(type=="ROOK") return GameFigure(Color, 'R', pos, std::make_unique<RookMovment>());
        else if(type=="BISHOP") return GameFigure(Color, 'B', pos, std::make_unique<BishopMovment>());
        else if(type=="HORSE") return GameFigure(Color, 'H', pos, std::make_unique<KnightMovment>());
        else if(type=="QUEEN") return GameFigure(Color, 'Q', pos, std::make_unique<QueenMovment>());
        else if(type=="KING") return GameFigure(Color, 'K', pos, std::make_unique<KingMovment>());
        else{
            std::cout << "Kein Valide Figur eingegeben im json dokument" << "\n";
            return GameFigure(Color, 'F', pos, std::make_unique<RookMovment>());
        }
    }
}
