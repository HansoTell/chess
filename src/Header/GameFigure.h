#pragma once
#include "Move.h"
#include "Color.h"
#include "BoardView.h"
#include <string>
#include <memory>
#include <iostream>

namespace chess{
    class GameFigure
    {
    protected:
        Color m_Color;
        char m_FigureChar;   
        Position m_Position;
    public:
        GameFigure(Color Color, char figureChar, Position& pos);
        virtual ~GameFigure() = default;
        virtual bool isMoveLegal(const Move& move, const BoardView& boardView) const;
        virtual void printChar() const ;
    protected:
        virtual void printColor() const;
        virtual bool isAllowedDirection(const Move& Move) const = 0;
        virtual bool isPathClear( const Move& Move, const BoardView& BoardView) const = 0;
    };


    class Pawn : public GameFigure{
    private: 

        virtual bool isAllowedDirection(const Move& Move) const override;
        virtual bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
    public:
        Pawn(Color color, Position& pos);
    };

    class Rook : public GameFigure
    {
    private:

        virtual bool isAllowedDirection( const Move& Move) const override;
        virtual bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
    public:
        Rook(Color color, Position& pos);
    };

    
    class Bishop : public GameFigure
    {
    private:
        virtual bool isAllowedDirection(const Move& Move) const override;
        virtual bool isPathClear(const Move& Move,const BoardView& BoardView) const override;
    public:
        Bishop(Color color, Position& pos);
    };

    
    class Horse : public GameFigure
    {
    private:
        virtual bool isAllowedDirection(const Move& Move) const override;
        virtual bool isPathClear(const Move& Move,const BoardView& BoardView) const override;
    public:
        Horse(Color color, Position& pos);
    };


    class Queen : public GameFigure
    {
    private:
        virtual bool isAllowedDirection(const Move& Move) const override;
        virtual bool isPathClear(const Move& Move,const BoardView& BoardView) const override;
    public:
        Queen(Color color, Position& pos);
    };


    class King : public GameFigure
    {
    private:
        virtual bool isAllowedDirection(const Move& Move) const override;
        virtual bool isPathClear(const Move& Move,const BoardView& BoardView) const override;
    public:
        King(Color color, Position& pos);
    };



    std::unique_ptr<GameFigure> GameFigureFactory(const std::string& type, Color color, Position& pos);
}