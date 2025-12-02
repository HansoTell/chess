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
    }

    GameFigure GameFigureFactory(FigureType figureType, Color Color, Position& pos){
        switch (figureType)
        {
        case PAWN:
            return GameFigure(Color, pos, std::make_unique<PawnMovement>(), PAWN, JUMPING);
        case ROOK:
            return GameFigure(Color, pos, std::make_unique<RookMovement>(), ROOK, SLIDING);
        case BISHOP:
            return GameFigure(Color, pos, std::make_unique<BishopMovement>(), BISHOP, SLIDING);
        case KNIGHT:
            return GameFigure(Color, pos, std::make_unique<KnightMovement>(), KNIGHT, JUMPING);
        case QUEEN:
            return GameFigure(Color, pos, std::make_unique<QueenMovement>(), QUEEN, SLIDING);
        case KING:
            return GameFigure(Color, pos, std::make_unique<KingMovement>(), KING, JUMPING);
        }
        std::cout << "Kein Valide Figur eingegeben im json dokument" << "\n";
        return GameFigure(Color, pos, std::make_unique<RookMovement>(), QUEEN, JUMPING);
    }
}
