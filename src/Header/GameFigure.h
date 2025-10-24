#pragma once
#include "Move.h"
#include "Color.h"
#include "BoardView.h"
#include "MovementLogic.h"
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

    public:
        GameFigure(Color Color, char figureChar, Position& pos, std::unique_ptr<MovementLogic> MovmentLogic);
        GameFigure(const GameFigure& other);
        GameFigure(GameFigure&& other);
        ~GameFigure() = default;
        GameFigure& operator=(GameFigure&& other) = default;
        bool isMoveLegal(const Move& move, const BoardView& boardView) const { return m_MovmentLogic->isMoveLegal(move, boardView); }
        void printChar() const;
    private:
        void printColor() const;
    };


    GameFigure GameFigureFactory(const std::string& type, Color color, Position& pos);
}