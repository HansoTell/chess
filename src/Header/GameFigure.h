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
        Position m_Position;
        std::unique_ptr<MovementLogic> m_MovmentLogic;
        FigureType m_FigureType;
        MovementTypes m_MovementType;
        std::vector<Position> m_Threats;

    public:
        GameFigure(Color Color, Position& pos, std::unique_ptr<MovementLogic> MovmentLogic, FigureType figureType, MovementTypes movementType);
        GameFigure(const GameFigure& other);
        GameFigure(GameFigure&& other);
        ~GameFigure() = default;
        GameFigure& operator=(GameFigure&& other) = default;
        bool operator==(const GameFigure& other) const {return this->m_Position == other.m_Position; }
    public:
        MoveResult isMoveLegal(const Move& move, const BoardView& boardView, const GameState& GameState  ) const { return m_MovmentLogic->isMoveLegal(move, boardView, GameState); }
        void updateThreats(const BoardView& BoardView) { m_Threats =  m_MovmentLogic->getThreatendSquares(m_Position, BoardView, m_Color); }
        void setThreats(std::vector<Position>& threats) { m_Threats = threats; }
    public:
        MovementTypes getMovementType() const { return m_MovementType; }
        FigureType getFigureType() const { return m_FigureType; }
        const Position& getPosition() const { return m_Position; }
        void setPosition(Position pos) { m_Position = pos; }
        Color getColor() const { return m_Color; }
        std::vector<Position>& getThreatendSquares()  { return m_Threats; }
        const std::vector<Position> getThreatendSquares() const { return m_Threats; }

    };


    GameFigure GameFigureFactory(FigureType figureType, Color color, Position& pos);
}