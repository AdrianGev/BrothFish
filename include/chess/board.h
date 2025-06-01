#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "piece.h"
#include <array>
#include <string>
#include <vector>

namespace chess {

// Position on the board (file, rank)
struct Position {
    int file; // 0-7 (a-h)
    int rank; // 0-7 (1-8)
    
    Position() : file(0), rank(0) {}
    Position(int file, int rank) : file(file), rank(rank) {}
    
    bool isValid() const {
        return file >= 0 && file < 8 && rank >= 0 && rank < 8;
    }
    
    // Convert to algebraic notation (e.g., "e4")
    std::string toAlgebraic() const;
    
    // Create from algebraic notation
    static Position fromAlgebraic(const std::string& algebraic);
    
    bool operator==(const Position& other) const {
        return file == other.file && rank == other.rank;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

// A move from one position to another
struct Move {
    Position from;
    Position to;
    PieceType promotion; // For pawn promotion
    
    Move() : from(), to(), promotion(PieceType::EMPTY) {}
    Move(Position from, Position to, PieceType promotion = PieceType::EMPTY)
        : from(from), to(to), promotion(promotion) {}
    
    // Convert to algebraic notation (e.g., "e2e4")
    std::string toAlgebraic() const;
    
    // Create from algebraic notation
    static Move fromAlgebraic(const std::string& algebraic);
    
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && promotion == other.promotion;
    }
};

// The chess board
class Board {
private:
    std::array<std::array<Piece, 8>, 8> pieces;
    Color sideToMove;
    bool whiteCanCastleKingside;
    bool whiteCanCastleQueenside;
    bool blackCanCastleKingside;
    bool blackCanCastleQueenside;
    Position enPassantTarget;
    int halfMoveClock;
    int fullMoveNumber;

public:
    // Initialize an empty board
    Board();
    
    // Initialize from FEN string
    Board(const std::string& fen);
    
    // Get piece at position
    Piece getPiece(const Position& pos) const;
    
    // Set piece at position
    void setPiece(const Position& pos, const Piece& piece);
    
    // Get the side to move
    Color getSideToMove() const { return sideToMove; }
    
    // Set the side to move
    void setSideToMove(Color color) { sideToMove = color; }
    
    // Toggle the side to move
    void toggleSideToMove() { sideToMove = (sideToMove == Color::WHITE) ? Color::BLACK : Color::WHITE; }
    
    // Get the FEN string for the current board
    std::string toFEN() const;
    
    // Print the board to the console
    void print() const;
    
    // Make a move on the board
    bool makeMove(const Move& move);
    
    // Check if a position is under attack by a specific color
    bool isUnderAttack(const Position& pos, Color attackingColor) const;
    
    // Check if the current side to move is in check
    bool isInCheck() const;
    
    // Generate all legal moves for the current side to move
    std::vector<Move> generateLegalMoves() const;
    
    // Generate all pseudo-legal moves for a specific piece
    std::vector<Move> generatePseudoLegalMoves(const Position& pos) const;
    
    // Check if a move is legal
    bool isLegalMove(const Move& move) const;
    
private:
    // Generate pawn moves
    void generatePawnMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate knight moves
    void generateKnightMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate bishop moves
    void generateBishopMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate rook moves
    void generateRookMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate queen moves
    void generateQueenMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate king moves
    void generateKingMoves(const Position& pos, std::vector<Move>& moves) const;
    
    // Generate sliding piece moves (bishop, rook, queen)
    void generateSlidingMoves(const Position& pos, const std::vector<std::pair<int, int>>& directions, 
                             std::vector<Move>& moves) const;
};

} // namespace chess

#endif // CHESS_BOARD_H
