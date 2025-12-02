#include "GameFigure.h"


namespace chess{

    GameFigure::GameFigure(Color color, Position& pos, std::unique_ptr<MovementLogic> MovementLogic, FigureType figureType, MovementTypes movementType): 
        m_Color(color), 
        m_Position(pos), 
        m_MovmentLogic(std::move(MovementLogic)), 
        m_FigureType(figureType), 
        m_MovementType(movementType)
        {}
    GameFigure::GameFigure(const GameFigure& other) : 
        m_Color(other.m_Color), 
        m_Position(other.m_Position), 
        m_FigureChar(other.m_FigureChar), 
        m_MovmentLogic(other.m_MovmentLogic ? other.m_MovmentLogic->clone() : nullptr)
        {}
    GameFigure::GameFigure(GameFigure&& other) : 
        m_Color(other.m_Color), 
        m_Position(other.m_Position), 
        m_FigureChar(other.m_FigureChar), 
        m_MovmentLogic(std::move(other.m_MovmentLogic)),
        m_FigureType(other.m_FigureType),
        m_MovementType(other.m_MovementType),
        m_Threats(std::move(other.m_Threats))
    {
        other.m_Color = WHITE;
        other.m_Position.x = 0;
        other.m_Position.y = 0;
        other.m_FigureChar = 'F';
    }

    GameFigure GameFigureFactory(const std::string& type, Color Color, Position& pos){
        if(type == "PAWN") return GameFigure(Color, pos, std::make_unique<PawnMovement>(), PAWN, JUMPING);
        else if(type=="ROOK") return GameFigure(Color, pos, std::make_unique<RookMovement>(), ROOK, SLIDING);
        else if(type=="BISHOP") return GameFigure(Color, pos, std::make_unique<BishopMovement>(), BISHOP, SLIDING);
        else if(type=="HORSE") return GameFigure(Color, pos, std::make_unique<KnightMovement>(), KNIGHT, JUMPING);
        else if(type=="QUEEN") return GameFigure(Color, pos, std::make_unique<QueenMovement>(), QUEEN, SLIDING);
        else if(type=="KING") return GameFigure(Color, pos, std::make_unique<KingMovement>(), KING, JUMPING);
        else{
            std::cout << "Kein Valide Figur eingegeben im json dokument" << "\n";
            return GameFigure(Color, pos, std::make_unique<RookMovement>(), QUEEN, JUMPING);
        }
    }
}
