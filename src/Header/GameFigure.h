#pragma once
#include "Move.h"
#include "Color.h"
#include "BoardView.h"
#include "MovementLogic.h"
#include "FigureTypes.h"
#include "GameState.h"
#include "MoveResult.h"
#include "MovementTypes.h"
#include <string>
#include <memory>
#include <iostream>

namespace chess{


    class GameFigure
    {
    private:
        Color m_Color;
        char m_FigureChar;   
        Position m_Position;
        std::unique_ptr<MovementLogic> m_MovmentLogic;
        FigureType m_FigureType;
        MovementTypes m_MovementType;
        std::vector<Position> m_Threats;

    public:
        GameFigure(Color Color, char figureChar, Position& pos, std::unique_ptr<MovementLogic> MovmentLogic, FigureType figureType, MovementTypes movementType);
        GameFigure(const GameFigure& other);
        GameFigure(GameFigure&& other);
        ~GameFigure() = default;
        GameFigure& operator=(GameFigure&& other) = default;
    public:
        MoveResult isMoveLegal(const Move& move, const BoardView& boardView, const GameState& GameState  ) const { return m_MovmentLogic->isMoveLegal(move, boardView, GameState); }
        void updateThreats(const BoardView& BoardView) { m_Threats =  m_MovmentLogic->getThreatendSquares(m_Position, BoardView, m_Color); }
        void printChar() const;
    public:
        MovementTypes getMovementType() const { return m_MovementType; }
        FigureType getFigureType() const { return m_FigureType; }
        const Position& getPosition() const { return m_Position; }
        Color getColor() const { return m_Color; }
        const std::vector<Position>& getThreatendSquares() const { return m_Threats; }

    private:
        void printColor() const;
    };


    GameFigure GameFigureFactory(const std::string& type, Color color, Position& pos);
}