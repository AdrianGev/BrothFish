#include "chess/evaluate.h"

namespace chess {

int Evaluator::evaluate(const Board& board) const {
    // start with material evaluation
    int score = evaluateMaterial(board);
    
    // add center control evaluation
    score += evaluateCenterControl(board);
    
    // add other evaluation components
    score += evaluatePiecePositions(board);
    // score += evaluateMobility(board);
    // score += evaluatePawnStructure(board);
    // score += evaluateKingSafety(board);
    score += evaluateEarlyQueenDevelopment(board);
    score += evaluatePieceDevelopment(board);
    score += evaluateEarlyKingMovement(board);
    score += evaluateCastling(board);
    score += evaluatePawnDoubleMoves(board);
    
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
    int score = 0;
    
    // knight position evaluation - knights are better near the center
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            // only evaluate knights
            if (piece.getType() == PieceType::KNIGHT) {
                // calculate distance from center (center is between d4, d5, e4, e5)
                // file distance: how far from columns d and e (files 3,4)
                int fileDistFromCenter = std::min(abs(file - 3), abs(file - 4));
                // rank distance: how far from ranks 4 and 5 (ranks 3,4)
                int rankDistFromCenter = std::min(abs(rank - 3), abs(rank - 4));
                
                // total Manhattan distance from center (its like the hypot but it sounds smart)
                int distFromCenter = fileDistFromCenter + rankDistFromCenter;
                
                // knights get bonus points for being closer to center
                // max bonus of 3 points for being in the center, decreasing as they move away
                int positionBonus = 3 - distFromCenter;
                if (positionBonus < 0) positionBonus = 0; // no penalty for being far
                
                // apply bonus based on piece color
                if (piece.getColor() == Color::WHITE) {
                    score += positionBonus;
                } else {
                    score -= positionBonus;
                }
            }
        }
    }
    
    return score;
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

int Evaluator::evaluateEarlyQueenDevelopment(const Board& board) const {
    int score = 0;
    
    // check white queen position
    Position whiteQueenStartPos(3, 0); // d1
    bool whiteQueenFound = false;
    
    // check black queen position
    Position blackQueenStartPos(3, 7); // d8
    bool blackQueenFound = false;
    
    // scan the board to find the queens
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (piece.getType() == PieceType::QUEEN) {
                if (piece.getColor() == Color::WHITE) {
                    whiteQueenFound = true;
                    
                    // if white queen is not on its starting square, apply penalty
                    if (pos != whiteQueenStartPos) {
                        // calculate manhattan distance from starting square
                        int distance = abs(pos.file - whiteQueenStartPos.file) + 
                                      abs(pos.rank - whiteQueenStartPos.rank);
                        
                        // Penalty is higher for moving the queen early
                        // -15 points penalty for moving the queen
                        score -= 15;
                        
                        // Additional penalty for moving it far from home
                        score -= distance * 2;
                    }
                } else { // BLACK
                    blackQueenFound = true;
                    
                    // if black queen is not on its starting square, apply penalty
                    if (pos != blackQueenStartPos) {
                        // calculate manhattan distance from starting square
                        int distance = abs(pos.file - blackQueenStartPos.file) + 
                                      abs(pos.rank - blackQueenStartPos.rank);
                        
                        // Penalty is higher for moving the queen early
                        // +15 points penalty for moving the queen (positive for white's advantage)
                        score += 15;
                        
                        // Additional penalty for moving it far from home
                        score += distance * 2;
                    }
                }
            }
        }
    }
    
    return score;
}

int Evaluator::evaluatePieceDevelopment(const Board& board) const {
    int score = 0;
    
    // define starting positions for pieces
    // white pieces
    const Position whiteKnightStartPos[2] = {Position(1, 0), Position(6, 0)}; // b1, g1
    const Position whiteBishopStartPos[2] = {Position(2, 0), Position(5, 0)}; // c1, f1
    const Position whiteRookStartPos[2] = {Position(0, 0), Position(7, 0)};   // a1, h1
    
    // black pieces
    const Position blackKnightStartPos[2] = {Position(1, 7), Position(6, 7)}; // b8, g8
    const Position blackBishopStartPos[2] = {Position(2, 7), Position(5, 7)}; // c8, f8
    const Position blackRookStartPos[2] = {Position(0, 7), Position(7, 7)};   // a8, h8
    
    // define good development squares for minor pieces
    // Good knight squares in the opening
    const Position goodWhiteKnightSquares[] = {
        Position(2, 2), // c3
        Position(5, 2), // f3
        Position(3, 2), // d3
        Position(4, 2)  // e3
    };
    
    const Position goodBlackKnightSquares[] = {
        Position(2, 5), // c6
        Position(5, 5), // f6
        Position(3, 5), // d6
        Position(4, 5)  // e6
    };
    
    // Good bishop squares in the opening
    const Position goodWhiteBishopSquares[] = {
        Position(2, 2), // c3
        Position(5, 2), // f3
        Position(3, 1), // d2
        Position(4, 1), // e2
        Position(1, 2), // b3
        Position(6, 2)  // g3
    };
    
    const Position goodBlackBishopSquares[] = {
        Position(2, 5), // c6
        Position(5, 5), // f6
        Position(3, 6), // d7
        Position(4, 6), // e7
        Position(1, 5), // b6
        Position(6, 5)  // g6
    };
    
    // track pieces that have moved from starting position
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (piece.isEmpty()) {
                continue;
            }
            
            // check if the piece is under attack
            bool isUnderAttack = false;
            if (piece.getColor() == Color::WHITE) {
                isUnderAttack = board.isUnderAttack(pos, Color::BLACK);
            } else {
                isUnderAttack = board.isUnderAttack(pos, Color::WHITE);
            }
            
            // handle knights
            if (piece.getType() == PieceType::KNIGHT) {
                if (piece.getColor() == Color::WHITE) {
                    // check if knight is not on starting square
                    if (pos != whiteKnightStartPos[0] && pos != whiteKnightStartPos[1]) {
                        bool onGoodSquare = false;
                        
                        // check if knight is on a good development square
                        for (const auto& goodSquare : goodWhiteKnightSquares) {
                            if (pos == goodSquare) {
                                onGoodSquare = true;
                                break;
                            }
                        }
                        
                        // If knight is not on a good square and not under attack, apply penalty
                        if (!onGoodSquare && !isUnderAttack) {
                            // Penalize knight for being on a suboptimal square
                            // this indirectly penalizes moving the same piece multiple times
                            score -= 8;
                        }
                    }
                } else { // BLACK
                    // check if knight is not on starting square
                    if (pos != blackKnightStartPos[0] && pos != blackKnightStartPos[1]) {
                        bool onGoodSquare = false;
                        
                        // check if knight is on a good development square
                        for (const auto& goodSquare : goodBlackKnightSquares) {
                            if (pos == goodSquare) {
                                onGoodSquare = true;
                                break;
                            }
                        }
                        
                        // if knight is not on a good square and not under attack, apply penalty
                        if (!onGoodSquare && !isUnderAttack) {
                            // penalize knight for being on a suboptimal square
                            score += 8; // positive for white's advantage
                        }
                    }
                }
            }
            
            // handle bishops
            else if (piece.getType() == PieceType::BISHOP) {
                if (piece.getColor() == Color::WHITE) {
                    // check if bishop is not on starting square
                    if (pos != whiteBishopStartPos[0] && pos != whiteBishopStartPos[1]) {
                        bool onGoodSquare = false;
                        
                        // check if bishop is on a good development square
                        for (const auto& goodSquare : goodWhiteBishopSquares) {
                            if (pos == goodSquare) {
                                onGoodSquare = true;
                                break;
                            }
                        }
                        
                        // if bishop is not on a good square and not under attack, apply penalty
                        if (!onGoodSquare && !isUnderAttack) {
                            // penalize bishop for being on a suboptimal square
                            score -= 8;
                        }
                    }
                } else { // BLACK
                    // check if bishop is not on starting square
                    if (pos != blackBishopStartPos[0] && pos != blackBishopStartPos[1]) {
                        bool onGoodSquare = false;
                        
                        // check if bishop is on a good development square
                        for (const auto& goodSquare : goodBlackBishopSquares) {
                            if (pos == goodSquare) {
                                onGoodSquare = true;
                                break;
                            }
                        }
                        
                        // if bishop is not on a good square and not under attack, apply penalty
                        if (!onGoodSquare && !isUnderAttack) {
                            // penalize bishop for being on a suboptimal square
                            score += 8; // positive for white's advantage
                        }
                    }
                }
            }
            
            // handle rooks - they generally shouldn't move early unless there's a good reason (rb1 reference those who know)
            else if (piece.getType() == PieceType::ROOK) {
                if (piece.getColor() == Color::WHITE) {
                    // check if rook is not on starting square
                    if (pos != whiteRookStartPos[0] && pos != whiteRookStartPos[1]) {
                        // if rook is not under attack, apply penalty for early movement
                        if (!isUnderAttack) {
                            score -= 10;
                        }
                    }
                } else { // BLACK
                    // check if rook is not on starting square
                    if (pos != blackRookStartPos[0] && pos != blackRookStartPos[1]) {
                        // if rook is not under attack, apply penalty for early movement
                        if (!isUnderAttack) {
                            score += 10; // positive for white's advantage
                        }
                    }
                }
            }
        }
    }
    
    return score;
}

int Evaluator::evaluateEarlyKingMovement(const Board& board) const {
    int score = 0;
    
    // Define starting positions for kings
    const Position whiteKingStartPos(4, 0); // e1
    const Position blackKingStartPos(4, 7); // e8
    
    // Check castling rights
    bool whiteCanCastle = false;
    bool blackCanCastle = false;
    
    // Scan the board to find the kings
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (piece.getType() == PieceType::KING) {
                if (piece.getColor() == Color::WHITE) {
                    // If white king is not on its starting square
                    if (pos != whiteKingStartPos) {
                        // Check if king moved above first rank (rank > 0) but castling might still be possible
                        if (pos.rank > 0) {
                            // Heavy penalty for moving king above first rank before castling
                            // -50 points is a significant penalty (roughly half a pawn)
                            score -= 50;
                            
                            // Additional penalty based on how far the king moved vertically
                            score -= pos.rank * 10;
                        }
                    }
                } else { // BLACK
                    // If black king is not on its starting square
                    if (pos != blackKingStartPos) {
                        // Check if king moved below last rank (rank < 7) but castling might still be possible
                        if (pos.rank < 7) {
                            // Heavy penalty for moving king below last rank before castling
                            // +50 points is a significant penalty (positive for white's advantage)
                            score += 50;
                            
                            // Additional penalty based on how far the king moved vertically
                            score += (7 - pos.rank) * 10;
                        }
                    }
                }
            }
        }
    }
    
    return score;
}

int Evaluator::evaluateCastling(const Board& board) const {
    int score = 0;
    
    // Define starting positions for kings and rooks
    const Position whiteKingStartPos(4, 0);   // e1
    const Position blackKingStartPos(4, 7);   // e8
    
    // Kingside castling positions
    const Position whiteKingsideCastlePos(6, 0);  // g1
    const Position blackKingsideCastlePos(6, 7);  // g8
    
    // Queenside castling positions
    const Position whiteQueensideCastlePos(2, 0); // c1
    const Position blackQueensideCastlePos(2, 7); // c8
    
    // Scan the board to find the kings
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (piece.getType() == PieceType::KING) {
                if (piece.getColor() == Color::WHITE) {
                    // Check if king has castled kingside
                    if (pos == whiteKingsideCastlePos) {
                        // Significant bonus for successful castling
                        score += 40;
                    }
                    // Check if king has castled queenside
                    else if (pos == whiteQueensideCastlePos) {
                        // Significant bonus for successful castling
                        score += 40;
                    }
                    // If king is still on starting square, give a small bonus for maintaining castling rights
                    else if (pos == whiteKingStartPos) {
                        // Check if rooks are still on their starting squares
                        Piece kingsideRook = board.getPiece(Position(7, 0)); // h1
                        Piece queensideRook = board.getPiece(Position(0, 0)); // a1
                        
                        // Bonus for maintaining kingside castling option
                        if (!kingsideRook.isEmpty() && kingsideRook.getType() == PieceType::ROOK && 
                            kingsideRook.getColor() == Color::WHITE) {
                            score += 15;
                        }
                        
                        // Bonus for maintaining queenside castling option
                        if (!queensideRook.isEmpty() && queensideRook.getType() == PieceType::ROOK && 
                            queensideRook.getColor() == Color::WHITE) {
                            score += 10; // Slightly less bonus for queenside as it's slightly less common
                        }
                    }
                } else { // BLACK
                    // Check if king has castled kingside
                    if (pos == blackKingsideCastlePos) {
                        // Significant bonus for successful castling (negative for white's advantage)
                        score -= 40;
                    }
                    // Check if king has castled queenside
                    else if (pos == blackQueensideCastlePos) {
                        // Significant bonus for successful castling (negative for white's advantage)
                        score -= 40;
                    }
                    // If king is still on starting square, give a small bonus for maintaining castling rights
                    else if (pos == blackKingStartPos) {
                        // Check if rooks are still on their starting squares
                        Piece kingsideRook = board.getPiece(Position(7, 7)); // h8
                        Piece queensideRook = board.getPiece(Position(0, 7)); // a8
                        
                        // Bonus for maintaining kingside castling option
                        if (!kingsideRook.isEmpty() && kingsideRook.getType() == PieceType::ROOK && 
                            kingsideRook.getColor() == Color::BLACK) {
                            score -= 15;
                        }
                        
                        // Bonus for maintaining queenside castling option
                        if (!queensideRook.isEmpty() && queensideRook.getType() == PieceType::ROOK && 
                            queensideRook.getColor() == Color::BLACK) {
                            score -= 10; // Slightly less bonus for queenside as it's slightly less common
                        }
                    }
                }
            }
        }
    }
    
    return score;
}

int Evaluator::evaluatePawnDoubleMoves(const Board& board) const {
    int score = 0;
    
    // Only apply this evaluation in the opening phase (first 10 moves)
    // We can estimate this by counting the total pieces on the board
    int pieceCount = 0;
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Piece piece = board.getPiece(Position(file, rank));
            if (!piece.isEmpty()) {
                pieceCount++;
            }
        }
    }
    
    // If we're not in the opening phase (too many pieces missing), return early
    if (pieceCount < 28) { // 32 pieces at start, allow for a few captures
        return score;
    }
    
    // Define starting positions for pawns
    const int whitePawnRank = 1; // 2nd rank
    const int blackPawnRank = 6; // 7th rank
    
    // Center squares (d4, d5, e4, e5)
    const Position centerSquares[4] = {
        Position(3, 3), Position(3, 4), Position(4, 3), Position(4, 4)
    };
    
    // Track pawns that have moved more than once
    for (int file = 0; file < 8; file++) {
        // Check white pawns
        bool whitePawnFound = false;
        
        // Check each rank to see if this file's pawn has moved
        for (int rank = 0; rank < 8; rank++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (!piece.isEmpty() && piece.getType() == PieceType::PAWN && piece.getColor() == Color::WHITE) {
                whitePawnFound = true;
                
                // Check if the pawn has moved from its starting position
                if (rank > whitePawnRank) {
                    // Check if the pawn has moved more than once
                    // A pawn on rank 3 (index 2) could have moved there in one move (e2-e4)
                    // But a pawn on rank 4 (index 3) or higher must have moved multiple times
                    if (rank > whitePawnRank + 2) {
                        // Check if the pawn is under attack - if so, we don't penalize movement
                        bool isUnderAttack = board.isUnderAttack(pos, Color::BLACK);
                        
                        if (!isUnderAttack) {
                            // Base penalty for moving a pawn twice
                            score -= 20;
                            
                            // Extra penalty for center pawns (d and e files)
                            if (file == 3 || file == 4) {
                                score -= 10;
                            }
                            
                            // Check if this pawn controls any center squares
                            bool controlsCenter = false;
                            for (const auto& centerPos : centerSquares) {
                                // Pawns control squares diagonally in front of them
                                if (abs(pos.file - centerPos.file) == 1 && 
                                    centerPos.rank == pos.rank + 1) {
                                    controlsCenter = true;
                                    break;
                                }
                            }
                            
                            // Extra penalty if it lost center control
                            if (!controlsCenter && (file == 2 || file == 3 || file == 4 || file == 5)) {
                                score -= 10;
                            }
                        }
                    }
                }
                
                break; // Found the pawn for this file, no need to check further ranks
            }
        }
        
        // Check black pawns
        bool blackPawnFound = false;
        
        // Check each rank to see if this file's pawn has moved
        for (int rank = 0; rank < 8; rank++) {
            Position pos(file, rank);
            Piece piece = board.getPiece(pos);
            
            if (!piece.isEmpty() && piece.getType() == PieceType::PAWN && piece.getColor() == Color::BLACK) {
                blackPawnFound = true;
                
                // Check if the pawn has moved from its starting position
                if (rank < blackPawnRank) {
                    // Check if the pawn has moved more than once
                    // A pawn on rank 4 (index 4) could have moved there in one move (e7-e5)
                    // But a pawn on rank 3 (index 3) or lower must have moved multiple times
                    if (rank < blackPawnRank - 2) {
                        // Check if the pawn is under attack - if so, we don't penalize movement
                        bool isUnderAttack = board.isUnderAttack(pos, Color::WHITE);
                        
                        if (!isUnderAttack) {
                            // Base penalty for moving a pawn twice
                            score += 20; // positive for white's advantage
                            
                            // Extra penalty for center pawns (d and e files)
                            if (file == 3 || file == 4) {
                                score += 10;
                            }
                            
                            // Check if this pawn controls any center squares
                            bool controlsCenter = false;
                            for (const auto& centerPos : centerSquares) {
                                // Pawns control squares diagonally in front of them
                                if (abs(pos.file - centerPos.file) == 1 && 
                                    centerPos.rank == pos.rank - 1) {
                                    controlsCenter = true;
                                    break;
                                }
                            }
                            
                            // Extra penalty if it lost center control
                            if (!controlsCenter && (file == 2 || file == 3 || file == 4 || file == 5)) {
                                score += 10;
                            }
                        }
                    }
                }
                
                break; // Found the pawn for this file, no need to check further ranks
            }
        }
    }
    
    return score;
}

} // namespace chess
