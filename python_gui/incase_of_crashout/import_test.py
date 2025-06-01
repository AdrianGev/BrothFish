#!/usr/bin/env python3
import os
import sys

# add parent directory to python path to find the chess_engine module
sys.path.insert(0, os.path.abspath('..'))

try:
    import chess_engine
    print(f"successfully imported chess_engine from {chess_engine.__file__}")
    print(f"available functions: {dir(chess_engine)}")
    
    # test evaluate_position
    start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    score = chess_engine.evaluate_position(start_fen)
    print(f"evaluation score for starting position: {score}")
    
    # test get_best_move
    move = chess_engine.get_best_move(start_fen, 2)
    print(f"best move for starting position: {move}")
    
except ImportError as e:
    print(f"failed to import chess_engine: {e}")
except Exception as e:
    print(f"error: {e}")
    import traceback
    traceback.print_exc()
