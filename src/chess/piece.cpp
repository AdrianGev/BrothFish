#include "chess/piece.h"
#include <cctype>

namespace chess {

char Piece::toChar() const {
    if (type == PieceType::EMPTY) {
        return ' ';
    }
    
    char c;
    switch (type) {
        case PieceType::PAWN:   c = 'P'; break;
        case PieceType::KNIGHT: c = 'N'; break;
        case PieceType::BISHOP: c = 'B'; break;
        case PieceType::ROOK:   c = 'R'; break;
        case PieceType::QUEEN:  c = 'Q'; break;
        case PieceType::KING:   c = 'K'; break;
        default:                c = '?'; break;
    }
    
    return (color == Color::WHITE) ? c : std::tolower(c);
}

int Piece::getValue() const {
    switch (type) {
        case PieceType::PAWN:   return 100;
        case PieceType::KNIGHT: return 320;
        case PieceType::BISHOP: return 330;
        case PieceType::ROOK:   return 500;
        case PieceType::QUEEN:  return 900;
        case PieceType::KING:   return 20000; // agressive
        default:                return 0;
    }
}

char Piece::toFEN() const {
    if (type == PieceType::EMPTY) {
        return ' ';
    }
    
    char c;
    switch (type) {
        case PieceType::PAWN:   c = 'p'; break;
        case PieceType::KNIGHT: c = 'n'; break;
        case PieceType::BISHOP: c = 'b'; break;
        case PieceType::ROOK:   c = 'r'; break;
        case PieceType::QUEEN:  c = 'q'; break;
        case PieceType::KING:   c = 'k'; break;
        default:                c = '?'; break;
    }
    
    return (color == Color::WHITE) ? std::toupper(c) : c;
}

Piece Piece::fromFEN(char fen) {
    if (fen == ' ' || fen == '.') {
        return Piece();
    }
    
    Color color = std::islower(fen) ? Color::BLACK : Color::WHITE;
    char c = std::tolower(fen);
    
    PieceType type;
    switch (c) {
        case 'p': type = PieceType::PAWN;   break;
        case 'n': type = PieceType::KNIGHT; break;
        case 'b': type = PieceType::BISHOP; break;
        case 'r': type = PieceType::ROOK;   break;
        case 'q': type = PieceType::QUEEN;  break;
        case 'k': type = PieceType::KING;   break;
        default:  type = PieceType::EMPTY;  break;
    }
    
    return Piece(type, color);
}

} // namespace chess :thumbsup:
