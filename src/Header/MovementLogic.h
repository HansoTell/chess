#pragma once
#include "Move.h"
#include "BoardView.h"
#include <math.h>
#include <memory>

namespace chess{

    class MovementLogic{
    public:
        virtual ~MovementLogic() = default;
        virtual bool isMoveLegal(const Move& Move, const BoardView& BoardView) const = 0; 
        virtual std::unique_ptr<MovementLogic> clone() const = 0;
    };

    class KingMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KingMovment>(*this); }
    };

    class QueenMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<QueenMovment>(*this); }
    };

    class RookMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<RookMovment>(*this); }
    private:
        bool isAllowedDirection(const Move& Move) const;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const;
    };

    class BishopMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<BishopMovment>(*this); }
    private:
        bool isAllowedDirection(const Move& Move) const;
        bool isPathClear(const Move& Move, const BoardView& BoardView) const;
    };

    class KnightMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KnightMovment>(*this); }
    };

    class PawnMovment : public MovementLogic{
    public:
        bool isMoveLegal(const Move& Move, const BoardView& BoardView) const override; 
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<PawnMovment>(*this); }
    };
}