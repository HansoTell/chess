#pragma once
#include "Move.h"
#include "BoardView.h"
#include "FigureTypes.h"
#include "GameState.h"
#include "MoveResult.h"
#include "MoveType.h"
#include "AttackTabels.h"
#include <math.h>
#include <algorithm>
#include <memory>

namespace chess{
    class GameFigure;


    class MovementLogic{
    public:
        virtual ~MovementLogic() = default;
        virtual MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const = 0; 
        virtual std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const = 0;
        virtual std::unique_ptr<MovementLogic> clone() const = 0;
    };

    class KingMovement : public MovementLogic{
    public:
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KingMovement>(*this); }
    };

    class QueenMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<QueenMovement>(*this); }
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
    };

    class RookMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<RookMovement>(*this); }
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
    };

    class BishopMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<BishopMovement>(*this); }
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
    };

    class KnightMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<KnightMovement>(*this); }
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
    };

    class PawnMovement : public MovementLogic{
    public:
        std::unique_ptr<MovementLogic> clone() const override { return std::make_unique<PawnMovement>(*this); }
        MoveResult isMoveLegal(const Move& move, const BoardView& BoardView, const GameState& GameState) const override;
        std::vector<Position> getThreatendSquares(const Position figPos, const BoardView& BoardView, Color color) const;
    protected:
        bool isDiagonalPathClear(const Move& move, const BoardView& BoardView) const { return BoardView.getFigureAt(move.m_DesiredPosition); }
    };
}