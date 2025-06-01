#include "chess/evaluate.h"

namespace chess {

int Evaluator::evaluate(const Board& board) const {
    // start with material evaluation
    int score = evaluateMaterial(board);
    
    // add center control evaluation
    score += evaluateCenterControl(board);
    
    // add other evaluation components (commented out for now)
    // score += evaluatePiecePositions(board);
    // score += evaluateMobility(board);
    // score += evaluatePawnStructure(board);
    // score += evaluateKingSafety(board);
    
    return score;
}

int Evaluator::evaluateMaterial(const Board& board) const {
    int score = 0;
    
    // material evaluation
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Piece piece = board.getPiece(Position(file, rank));
            if (!piece.isEmpty()) {
                int value = piece.getValue();
                if (piece.getColor() == Color::BLACK) {
                    value = -value;
                }
                score += value;
            }
        }
    }
    
    return score;
}

int Evaluator::evaluatePiecePositions(const Board& board) const {
    // todo
    return 0;
}

int Evaluator::evaluateCenterControl(const Board& board) const {
    int score = 0;
    
    Position centerSquares[4] = {
        Position(3, 3), // d4
        Position(3, 4), // d5
        Position(4, 3), // e4
        Position(4, 4)  // e5
    };
    
    // bonus for occupying center squares
    for (const auto& pos : centerSquares) {
        Piece piece = board.getPiece(pos);
        if (!piece.isEmpty()) {
            // bonus for having pieces in the center
            int bonus = (piece.getType() == PieceType::PAWN) ? 20 : 10;
            
            // apply bonus based on piece color
            if (piece.getColor() == Color::WHITE) {
                score += bonus;
            } else {
                score -= bonus;
            }
            
            // check if the piece is under attack
            if (board.isUnderAttack(pos, piece.getColor() == Color::WHITE ? Color::BLACK : Color::WHITE)) {
                // penalize if the piece is under attack
                if (piece.getColor() == Color::WHITE) {
                    score -= 5;
                } else {
                    score += 5;
                }
            }
        }
    }
    
    // bonus for attacking center squares
    for (const auto& pos : centerSquares) {
        // check white's control
        if (board.isUnderAttack(pos, Color::WHITE)) {
            score += 5;
        }
        
        // check black's control
        if (board.isUnderAttack(pos, Color::BLACK)) {
            score -= 5;
        }
    }
    
    return score;
}

int Evaluator::evaluateMobility(const Board& board) const {
    // todo
    return 0;
}

int Evaluator::evaluatePawnStructure(const Board& board) const {
    // todo
    return 0;
}

int Evaluator::evaluateKingSafety(const Board& board) const {
    // todo
    return 0;
}

} // namespace chess
