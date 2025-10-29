#pragma once
#include "Move.h"
#include "BoardView.h"
#include "FigureTypes.h"
#include "GameFigure.h"
#include <math.h>
#include <memory>

namespace chess{

    class MovementLogic{
    public:
        virtual ~MovementLogic() = default;
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const; 
        virtual std::unique_ptr<MovementLogic> clone() const = 0;
    protected:
        virtual bool isAllowedDirection(const Move& Move) const = 0;
        virtual bool isPathClear(const Move& Move, const BoardView& BoardView) const = 0;
    };

    class KingMovement : public MovementLogic{
    public:
        bool wasInCheck = false;
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KingMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override { return true; }
    };

    class QueenMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<QueenMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
    };

    class RookMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<RookMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
    };

    class BishopMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<BishopMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
    };

    class KnightMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KnightMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override { return true; }
    };

    class PawnMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<PawnMovement>(*this); }
    protected:
        bool isAllowedDirection(const Move& Move) const override;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const override;
        bool isDiagonalPathClear(const Move& move, const BoardView& BoardView) const { return BoardView.getFigureAt(move.m_DesiredPosition); }
    };
}