#!/usr/bin/env python3

# simple test script for the C++ chess engine binding

import sys
import time
import os

# add parent directory to Python path to find the chess_engine module
sys.path.insert(0, os.path.abspath('..'))

print(f"Python version: {sys.version}")
print("Attempting to import chess_engine module...")

try:
    import chess_engine
    print("Successfully imported chess_engine module!")
    
    # test the get_best_move function with a standard starting position
    start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    print(f"\nTesting get_best_move with starting position:")
    print(f"FEN: {start_fen}")
    
    start_time = time.time()
    move = chess_engine.get_best_move(start_fen, 3)
    elapsed = time.time() - start_time
    
    print(f"Best move: {move}")
    print(f"Calculation time: {elapsed:.2f} seconds")
    
    # test the evaluate_position function
    print(f"\nTesting evaluate_position with starting position:")
    eval_score = chess_engine.evaluate_position(start_fen)
    print(f"Evaluation score: {eval_score}")
    
    # test with a more complex position
    mid_game_fen = "r1bqkb1r/pppp1ppp/2n2n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4"
    print(f"\nTesting with mid-game position:")
    print(f"FEN: {mid_game_fen}")
    
    start_time = time.time()
    move = chess_engine.get_best_move(mid_game_fen, 3)
    elapsed = time.time() - start_time
    
    print(f"Best move: {move}")
    print(f"Calculation time: {elapsed:.2f} seconds")
    
    eval_score = chess_engine.evaluate_position(mid_game_fen)
    print(f"Evaluation score: {eval_score}")
    
except ImportError as e:
    print(f"Failed to import chess_engine module: {e}")
except Exception as e:
    print(f"Error during testing: {e}")
    import traceback
    traceback.print_exc()
