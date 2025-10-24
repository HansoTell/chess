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
