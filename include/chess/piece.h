#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>

namespace chess {

enum class PieceType {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum class Color {
    NONE,
    WHITE,
    BLACK
};

/**
 * @brief Represents a chess piece
 */
class Piece {
private:
    PieceType type;
    Color color;

public:
    Piece() : type(PieceType::EMPTY), color(Color::NONE) {}
    Piece(PieceType type, Color color) : type(type), color(color) {}

    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    
    bool isEmpty() const { return type == PieceType::EMPTY; }
    
    // Get the character representation of the piece
    char toChar() const;
    
    // Get the value of the piece for evaluation
    int getValue() const;
    
    // Get the FEN character for the piece
    char toFEN() const;
    
    // Create a piece from a FEN character
    static Piece fromFEN(char fen);
};

} // namespace chess

#endif // CHESS_PIECE_H
