#!/usr/bin/env python3

# simple test script for the chess_engine module

try:
    import chess_engine
    print("successfully imported chess_engine module! lets freaking go dude!")
except Exception as e:
    print(f"Error: {e}")
    import traceback
    traceback.print_exc()