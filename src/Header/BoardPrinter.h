#pragma once

#include <iostream>

#include "BoardView.h"
#include "Constants.h"
#include "FigureTypes.h"
#include "Move.h"
#include "GameFigure.h"

namespace chess{
    class BoardPrinter
    {
    public:
        virtual FigureType getPromotionFigure() const = 0;
        virtual void printBoard(const BoardView& BoardView) const = 0;
    };


    class ASCIIPrinter : public BoardPrinter
    {
    public:
        FigureType getPromotionFigure() const override;
        void printBoard(const BoardView& BoardView) const override;
    private:
        void printHeader() const;
        void printFooter() const;
        void printFigure(FigureType figureType, Color color) const ;
    };

}