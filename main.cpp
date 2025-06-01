#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include "chess/board.h"
#include "chess/engine.h"
#include "chess/gui.h"

using namespace chess;

int main() {
    // initialize a board with the starting position
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    // create an engine with depth 2 (looks 2 moves ahead)
    Engine engine(2);
    
    // init the gooey
    GUI gui(800);
    if (!gui.initialize()) {
        std::cerr << "failed to initialize GUI womp womp" << std::endl;
        return 1;
    }
    
    std::cout << "welcome to BrothFish chess engine, bro is gonna get cooked" << std::endl;
    std::cout << "you are playing as white, the computer plays as black" << std::endl;
    std::cout << "click on a piece and then click on a destination square to move" << std::endl;
    
    bool gameOver = false;
    
    while (!gameOver && gui.isOpen()) {
        // draw the current board (no duh)
        gui.drawBoard(board);
        
        // check for checkmate or stalemate
        std::vector<Move> legalMoves = board.generateLegalMoves();
        if (legalMoves.empty()) {
            if (board.isInCheck()) {
                std::cout << "checkmate! ezzz " << (board.getSideToMove() == Color::WHITE ? "Black" : "White") << " wins!" << std::endl;
            } else {
                std::cout << "stalemate! (the game is a draw), dont worry, it's not about winning, it's about the friends we made along the way" << std::endl;
            }
            gameOver = true;
            
            // keep the window open for a moment so the user can see how they got cooked
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }
        
        if (board.getSideToMove() == Color::WHITE) {
            // player's turn (white)
            Move playerMove = gui.processEvents(board);
            
            if (!gui.isOpen()) {
                // window was closed
                break;
            }
            
            // only process the move if it's valid (both from and to positions are valid)
            if (playerMove.from.isValid() && playerMove.to.isValid()) {
                // check if this is a capture
                Piece targetPiece = board.getPiece(playerMove.to);
                if (!targetPiece.isEmpty()) {
                    std::cout << "capturing piece: " << targetPiece.toChar() << " at " 
                              << static_cast<char>('a' + playerMove.to.file) << (playerMove.to.rank + 1) << std::endl;
                }
                
                // make the player's move
                bool moveSuccess = board.makeMove(playerMove);
                if (moveSuccess) {
                    std::cout << "you moved: " << playerMove.toAlgebraic() << std::endl;
                } else {
                    std::cout << "invalid move: " << playerMove.toAlgebraic() << std::endl;
                }
            }
        } else {
            // computer's turn (black)
            std::cout << "computer is thinking..." << std::endl; // makes the chaf look cool
            Move computerMove = engine.getBestMove(board);
            board.makeMove(computerMove);
            std::cout << "computer moved: " << computerMove.toAlgebraic() << std::endl;
            
            // small delay to make the computer's move visible
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    
    return 0;
}