#include "chess/board.h"
#include <iostream>

namespace chess {

bool Board::makeMove(const Move& move) {
    // check if the move is valid
    if (!move.from.isValid() || !move.to.isValid()) {
        return false;
    }
    
    // get the piece at the from position
    Piece piece = getPiece(move.from);
    
    // check if there is a piece at the from position and it's the correct color
    if (piece.isEmpty() || piece.getColor() != sideToMove) {
        return false;
    }
    
    // check if the move is legal
    if (!isLegalMove(move)) {
        return false;
    }
    
    // handle captures
    Piece targetPiece = getPiece(move.to);
    bool isCapture = !targetPiece.isEmpty();
    
    // make the move
    setPiece(move.to, piece);
    setPiece(move.from, Piece());
    
    // handle pawn promotion
    if (piece.getType() == PieceType::PAWN && 
        ((sideToMove == Color::WHITE && move.to.rank == 7) || 
         (sideToMove == Color::BLACK && move.to.rank == 0))) {
        if (move.promotion != PieceType::EMPTY) {
            setPiece(move.to, Piece(move.promotion, sideToMove));
        } else {
            // default promotion to queen idc man
            setPiece(move.to, Piece(PieceType::QUEEN, sideToMove));
        }
    }
    
    // toggle side to move
    toggleSideToMove();
    
    return true;
}

bool Board::isUnderAttack(const Position& pos, Color attackingColor) const {
    // check for pawn attacks
    int pawnDirection = (attackingColor == Color::WHITE) ? 1 : -1;
    Position leftAttack(pos.file - 1, pos.rank - pawnDirection);
    Position rightAttack(pos.file + 1, pos.rank - pawnDirection);
    
    if (leftAttack.isValid()) {
        Piece attacker = getPiece(leftAttack);
        if (!attacker.isEmpty() && attacker.getColor() == attackingColor && 
            attacker.getType() == PieceType::PAWN) {
            return true;
        }
    }
    
    if (rightAttack.isValid()) {
        Piece attacker = getPiece(rightAttack);
        if (!attacker.isEmpty() && attacker.getColor() == attackingColor && 
            attacker.getType() == PieceType::PAWN) {
            return true;
        }
    }
    
    // check for knight attacks
    const std::vector<std::pair<int, int>> knightMoves = { // vectors of pairs of int
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (const auto& move : knightMoves) {
        Position knightPos(pos.file + move.first, pos.rank + move.second);
        if (knightPos.isValid()) {
            Piece attacker = getPiece(knightPos);
            if (!attacker.isEmpty() && attacker.getColor() == attackingColor && 
                attacker.getType() == PieceType::KNIGHT) {
                return true;
            }
        }
    }
    
    // check for sliding piece attacks (bishop, rook, queen) (smooth criminal)
    const std::vector<std::pair<int, int>> bishopDirections = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    
    const std::vector<std::pair<int, int>> rookDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    // check bishop-like moves (bishop, queen) (diags)
    for (const auto& dir : bishopDirections) {
        for (int i = 1; i < 8; i++) {
            Position checkPos(pos.file + i * dir.first, pos.rank + i * dir.second);
            if (!checkPos.isValid()) {
                break;
            }
            
            Piece piece = getPiece(checkPos);
            if (!piece.isEmpty()) {
                if (piece.getColor() == attackingColor && 
                    (piece.getType() == PieceType::BISHOP || piece.getType() == PieceType::QUEEN)) {
                    return true;
                }
                break;
            }
        }
    }
    
    // check rook-like moves (rook, queen)
    for (const auto& dir : rookDirections) {
        for (int i = 1; i < 8; i++) {
            Position checkPos(pos.file + i * dir.first, pos.rank + i * dir.second);
            if (!checkPos.isValid()) {
                break;
            }
            
            Piece piece = getPiece(checkPos);
            if (!piece.isEmpty()) {
                if (piece.getColor() == attackingColor && 
                    (piece.getType() == PieceType::ROOK || piece.getType() == PieceType::QUEEN)) {
                    return true;
                }
                break;
            }
        }
    }
    
    // check for king attacks
    const std::vector<std::pair<int, int>> kingMoves = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    
    for (const auto& move : kingMoves) {
        Position kingPos(pos.file + move.first, pos.rank + move.second);
        if (kingPos.isValid()) {
            Piece attacker = getPiece(kingPos);
            if (!attacker.isEmpty() && attacker.getColor() == attackingColor && 
                attacker.getType() == PieceType::KING) {
                return true;
            }
        }
    }
    
    return false;
}

bool Board::isInCheck() const {
    // find the king, hmmm
    Position kingPos(-1, -1);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Piece piece = pieces[rank][file];
            if (!piece.isEmpty() && piece.getType() == PieceType::KING && 
                piece.getColor() == sideToMove) {
                kingPos = Position(file, rank);
                break;
            }
        }
        if (kingPos.isValid()) {
            break;
        }
    }
    
    if (!kingPos.isValid()) {
        // this shouldn't happen in a valid chess position but like good code habits and stuff
        return false;
    }
    
    // check if the king is under attack
    return isUnderAttack(kingPos, (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

std::vector<Move> Board::generateLegalMoves() const {
    std::vector<Move> legalMoves;
    
    // generate moves for all pieces of the current side to move
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Position pos(file, rank);
            Piece piece = getPiece(pos);
            
            if (!piece.isEmpty() && piece.getColor() == sideToMove) {
                std::vector<Move> pieceMoves = generatePseudoLegalMoves(pos);
                
                // filter out moves that would leave the king in check
                for (const Move& move : pieceMoves) {
                    // make a copy of the board
                    Board testBoard = *this;
                    
                    // make the move on the copy
                    testBoard.setPiece(move.to, piece);
                    testBoard.setPiece(move.from, Piece());
                    
                    // handle pawn promotion
                    if (piece.getType() == PieceType::PAWN && 
                        ((sideToMove == Color::WHITE && move.to.rank == 7) || 
                         (sideToMove == Color::BLACK && move.to.rank == 0))) {
                        if (move.promotion != PieceType::EMPTY) {
                            testBoard.setPiece(move.to, Piece(move.promotion, sideToMove));
                        } else {
                            // default promotion to queen idc man
                            testBoard.setPiece(move.to, Piece(PieceType::QUEEN, sideToMove));
                        }
                    }
                    
                    // find the king on the test board
                    Position kingPos(-1, -1);
                    for (int r = 0; r < 8; r++) {
                        for (int f = 0; f < 8; f++) {
                            Piece p = testBoard.getPiece(Position(f, r));
                            if (!p.isEmpty() && p.getType() == PieceType::KING && 
                                p.getColor() == sideToMove) {
                                kingPos = Position(f, r);
                                break;
                            }
                        }
                        if (kingPos.isValid()) {
                            break;
                        }
                    }
                    
                    // check if the king is under attack after the move
                    if (!kingPos.isValid() || 
                        !testBoard.isUnderAttack(kingPos, (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE)) {
                        legalMoves.push_back(move);
                    }
                }
            }
        }
    }
    
    return legalMoves;
}

std::vector<Move> Board::generatePseudoLegalMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece piece = getPiece(pos);
    
    if (piece.isEmpty() || piece.getColor() != sideToMove) {
        return moves;
    }
    
    switch (piece.getType()) {
        case PieceType::PAWN:
            generatePawnMoves(pos, moves);
            break;
        case PieceType::KNIGHT:
            generateKnightMoves(pos, moves);
            break;
        case PieceType::BISHOP:
            generateBishopMoves(pos, moves);
            break;
        case PieceType::ROOK:
            generateRookMoves(pos, moves);
            break;
        case PieceType::QUEEN:
            generateQueenMoves(pos, moves);
            break;
        case PieceType::KING:
            generateKingMoves(pos, moves);
            break;
        default:
            break;
    }
    
    return moves;
}

bool Board::isLegalMove(const Move& move) const {
    // check if the move is valid
    if (!move.from.isValid() || !move.to.isValid()) {
        return false;
    }
    
    // get the piece at the from position
    Piece piece = getPiece(move.from);
    
    // check if there is a piece at the from position and it's the correct color
    if (piece.isEmpty() || piece.getColor() != sideToMove) {
        return false;
    }
    
    // get all legal moves for this piece
    std::vector<Move> legalMoves = generateLegalMoves();
    
    // check if the move is in the list of legal moves
    for (const Move& legalMove : legalMoves) {
        if (legalMove.from == move.from && legalMove.to == move.to) {
            return true;
        }
    }
    
    return false;
}

void Board::generatePawnMoves(const Position& pos, std::vector<Move>& moves) const {
    Piece pawn = getPiece(pos);
    if (pawn.isEmpty() || pawn.getType() != PieceType::PAWN) {
        return;
    }
    
    int direction = (pawn.getColor() == Color::WHITE) ? 1 : -1;
    int startRank = (pawn.getColor() == Color::WHITE) ? 1 : 6;
    int promotionRank = (pawn.getColor() == Color::WHITE) ? 7 : 0;
    
    // forward move
    Position forward(pos.file, pos.rank + direction);
    if (forward.isValid() && getPiece(forward).isEmpty()) {
        // check for promotion
        if (forward.rank == promotionRank) {
            moves.push_back(Move(pos, forward, PieceType::QUEEN));
            moves.push_back(Move(pos, forward, PieceType::ROOK));
            moves.push_back(Move(pos, forward, PieceType::BISHOP));
            moves.push_back(Move(pos, forward, PieceType::KNIGHT));
        } else {
            moves.push_back(Move(pos, forward));
        }
        
        // double forward move from starting position
        if (pos.rank == startRank) {
            Position doubleForward(pos.file, pos.rank + 2 * direction);
            if (doubleForward.isValid() && getPiece(doubleForward).isEmpty()) {
                moves.push_back(Move(pos, doubleForward));
            }
        }
    }
    
    // captures
    for (int fileOffset : {-1, 1}) {
        Position capture(pos.file + fileOffset, pos.rank + direction);
        if (capture.isValid()) {
            Piece target = getPiece(capture);
            if (!target.isEmpty() && target.getColor() != pawn.getColor()) {
                // check for promotion
                if (capture.rank == promotionRank) {
                    moves.push_back(Move(pos, capture, PieceType::QUEEN));
                    moves.push_back(Move(pos, capture, PieceType::ROOK));
                    moves.push_back(Move(pos, capture, PieceType::BISHOP));
                    moves.push_back(Move(pos, capture, PieceType::KNIGHT));
                } else {
                    moves.push_back(Move(pos, capture));
                }
            }
        }
    }
}

void Board::generateKnightMoves(const Position& pos, std::vector<Move>& moves) const {
    Piece knight = getPiece(pos);
    if (knight.isEmpty() || knight.getType() != PieceType::KNIGHT) {
        return;
    }
    
    const std::vector<std::pair<int, int>> knightMoves = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (const auto& move : knightMoves) {
        Position target(pos.file + move.first, pos.rank + move.second);
        if (target.isValid()) {
            Piece targetPiece = getPiece(target);
            if (targetPiece.isEmpty() || targetPiece.getColor() != knight.getColor()) {
                moves.push_back(Move(pos, target));
            }
        }
    }
}

void Board::generateBishopMoves(const Position& pos, std::vector<Move>& moves) const {
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    
    generateSlidingMoves(pos, directions, moves);
}

void Board::generateRookMoves(const Position& pos, std::vector<Move>& moves) const {
    const std::vector<std::pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    
    generateSlidingMoves(pos, directions, moves);
}

void Board::generateQueenMoves(const Position& pos, std::vector<Move>& moves) const {
    const std::vector<std::pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    
    generateSlidingMoves(pos, directions, moves);
}

void Board::generateKingMoves(const Position& pos, std::vector<Move>& moves) const {
    Piece king = getPiece(pos);
    if (king.isEmpty() || king.getType() != PieceType::KING) {
        return;
    }
    
    const std::vector<std::pair<int, int>> kingMoves = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    
    for (const auto& move : kingMoves) {
        Position target(pos.file + move.first, pos.rank + move.second);
        if (target.isValid()) {
            Piece targetPiece = getPiece(target);
            if (targetPiece.isEmpty() || targetPiece.getColor() != king.getColor()) {
                moves.push_back(Move(pos, target));
            }
        }
    }
}

void Board::generateSlidingMoves(const Position& pos, const std::vector<std::pair<int, int>>& directions, 
                                std::vector<Move>& moves) const {
    Piece piece = getPiece(pos);
    if (piece.isEmpty()) {
        return;
    }
    
    for (const auto& dir : directions) {
        for (int i = 1; i < 8; i++) {
            Position target(pos.file + i * dir.first, pos.rank + i * dir.second);
            if (!target.isValid()) {
                break;
            }
            
            Piece targetPiece = getPiece(target);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(pos, target));
            } else {
                if (targetPiece.getColor() != piece.getColor()) {
                    moves.push_back(Move(pos, target));
                }
                break;
            }
        }
    }
}

} // namespace chess
