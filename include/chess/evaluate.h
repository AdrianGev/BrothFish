#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#include "chess/board.h"

namespace chess {

class Evaluator {
public:
    // Constructor
    Evaluator() = default;
    
    // Main evaluation function
    int evaluate(const Board& board) const;
    
private:
    // Material evaluation
    int evaluateMaterial(const Board& board) const;
    
    // Center control evaluation
    int evaluateCenterControl(const Board& board) const;
    
    // Piece position evaluation (can be added later)
    int evaluatePiecePositions(const Board& board) const;
    
    // Mobility evaluation (can be added later)
    int evaluateMobility(const Board& board) const;
    
    // Pawn structure evaluation (can be added later)
    int evaluatePawnStructure(const Board& board) const;
    
    // King safety evaluation (can be added later)
    int evaluateKingSafety(const Board& board) const;
};

} // namespace chess

#endif // CHESS_EVALUATE_H
