#include "chess/board.h"
#include <iostream>
#include <sstream>

namespace chess {

// position methods
std::string Position::toAlgebraic() const {
    if (!isValid()) return "";
    return std::string(1, 'a' + file) + std::string(1, '1' + rank);
}

Position Position::fromAlgebraic(const std::string& algebraic) {
    if (algebraic.length() != 2) return Position(-1, -1);
    int file = algebraic[0] - 'a';
    int rank = algebraic[1] - '1';
    return Position(file, rank);
}

// move methods
std::string Move::toAlgebraic() const {
    std::string result = from.toAlgebraic() + to.toAlgebraic();
    if (promotion != PieceType::EMPTY) {
        char promotionChar = ' ';
        switch (promotion) {
            case PieceType::QUEEN:  promotionChar = 'q'; break;
            case PieceType::ROOK:   promotionChar = 'r'; break;
            case PieceType::BISHOP: promotionChar = 'b'; break;
            case PieceType::KNIGHT: promotionChar = 'n'; break;
            default: break;
        }
        result += promotionChar;
    }
    return result;
}

Move Move::fromAlgebraic(const std::string& algebraic) {
    if (algebraic.length() < 4) return Move();
    
    Position from = Position::fromAlgebraic(algebraic.substr(0, 2));
    Position to = Position::fromAlgebraic(algebraic.substr(2, 2));
    
    PieceType promotion = PieceType::EMPTY;
    if (algebraic.length() > 4) {
        char promotionChar = algebraic[4];
        switch (promotionChar) {
            case 'q': promotion = PieceType::QUEEN;  break;
            case 'r': promotion = PieceType::ROOK;   break;
            case 'b': promotion = PieceType::BISHOP; break;
            case 'n': promotion = PieceType::KNIGHT; break;
            default: break;
        }
    }
    
    return Move(from, to, promotion);
}

// board methods
Board::Board() : sideToMove(Color::WHITE), 
                whiteCanCastleKingside(false), whiteCanCastleQueenside(false),
                blackCanCastleKingside(false), blackCanCastleQueenside(false),
                halfMoveClock(0), fullMoveNumber(1) {
    // init mt board
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            pieces[rank][file] = Piece();
        }
    }
}

Board::Board(const std::string& fen) : Board() {
    std::istringstream ss(fen);
    std::string boardStr, activeColor, castling, enPassant, halfMove, fullMove;
    
    // prse board position
    ss >> boardStr;
    int rank = 7;
    int file = 0;
    
    for (char c : boardStr) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (std::isdigit(c)) {
            file += c - '0';
        } else {
            pieces[rank][file] = Piece::fromFEN(c);
            file++;
        }
    }
    
    // parse active color
    ss >> activeColor;
    sideToMove = (activeColor == "w") ? Color::WHITE : Color::BLACK;
    
    // parse castling rights
    ss >> castling;
    whiteCanCastleKingside = (castling.find('K') != std::string::npos);
    whiteCanCastleQueenside = (castling.find('Q') != std::string::npos);
    blackCanCastleKingside = (castling.find('k') != std::string::npos);
    blackCanCastleQueenside = (castling.find('q') != std::string::npos);
    
    // parse en passant target square
    ss >> enPassant;
    if (enPassant != "-") {
        enPassantTarget = Position::fromAlgebraic(enPassant);
    }
    
    // parse half move clock and full move number
    ss >> halfMove >> fullMove;
    halfMoveClock = std::stoi(halfMove);
    fullMoveNumber = std::stoi(fullMove);
}

Piece Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return Piece();
    return pieces[pos.rank][pos.file];
}

void Board::setPiece(const Position& pos, const Piece& piece) {
    if (!pos.isValid()) return;
    pieces[pos.rank][pos.file] = piece;
}

std::string Board::toFEN() const {
    std::stringstream ss;
    
    // board position
    for (int rank = 7; rank >= 0; rank--) {
        int emptyCount = 0;
        
        for (int file = 0; file < 8; file++) {
            Piece piece = pieces[rank][file];
            
            if (piece.isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    ss << emptyCount;
                    emptyCount = 0;
                }
                ss << piece.toFEN();
            }
        }
        
        if (emptyCount > 0) {
            ss << emptyCount;
        }
        
        if (rank > 0) {
            ss << '/';
        }
    }
    
    // active color
    ss << ' ' << (sideToMove == Color::WHITE ? 'w' : 'b');
    
    // castling rights
    ss << ' ';
    if (whiteCanCastleKingside || whiteCanCastleQueenside || 
        blackCanCastleKingside || blackCanCastleQueenside) {
        if (whiteCanCastleKingside) ss << 'K';
        if (whiteCanCastleQueenside) ss << 'Q';
        if (blackCanCastleKingside) ss << 'k';
        if (blackCanCastleQueenside) ss << 'q';
    } else {
        ss << '-';
    }
    
    // en passant target square
    ss << ' ';
    if (enPassantTarget.isValid()) {
        ss << enPassantTarget.toAlgebraic();
    } else {
        ss << '-';
    }
    
    // half move clock and full move number
    ss << ' ' << halfMoveClock << ' ' << fullMoveNumber;
    
    return ss.str();
}

void Board::print() const {
    std::cout << "  a b c d e f g h" << std::endl;
    std::cout << " +-+-+-+-+-+-+-+-+" << std::endl;
    
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "|";
        
        for (int file = 0; file < 8; file++) {
            std::cout << pieces[rank][file].toChar() << "|";
        }
        
        std::cout << rank + 1 << std::endl;
        std::cout << " +-+-+-+-+-+-+-+-+" << std::endl;
    }
    
    std::cout << "  a b c d e f g h" << std::endl;
    std::cout << "Side to move: " << (sideToMove == Color::WHITE ? "White" : "Black") << std::endl;
}



} // namespace chess
