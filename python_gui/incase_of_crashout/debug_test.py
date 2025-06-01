#!/usr/bin/env python3

# debug test script for the chess_engine module

import sys
import traceback

def main():
    print(f"python ver: {sys.version}")
    print("attempting to import chess_engine module...")
    
    try:
        import chess_engine
        print("successfully imported chess_engine module!")
        
        # test the get_best_move function with a standard starting position
        start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        print(f"\nTesting get_best_move with starting position:")
        print(f"FEN: {start_fen}")
        
        move = chess_engine.get_best_move(start_fen, 2)  # using depth 2 for faster testing
        print(f"Best move: {move}")
        
        # test the evaluate_position function
        print(f"\nTesting evaluate_position with starting position:")
        eval_score = chess_engine.evaluate_position(start_fen)
        print(f"Evaluation score: {eval_score}")
        
        print("\nall tests passed successfully! (euphoria)")
        
    except ImportError as e:
        print(f"failed to import chess_engine module: {e}")
        traceback.print_exc()
    except Exception as e:
        print(f"error during testing: {e}")
        traceback.print_exc()

if __name__ == "__main__":
    main()
