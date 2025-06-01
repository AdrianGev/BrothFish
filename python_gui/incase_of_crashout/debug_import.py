#!/usr/bin/env python3

# debug script for importing the chess_engine module

import sys
import os

print(f"python ver: {sys.version}")
print(f"current directory: {os.getcwd()}")
print(f"files in current directory: {os.listdir('.')}")
print("attempting to import chess_engine module...")

try:
    import chess_engine
    print("successfully imported chess_engine module")
    print(f"module location: {chess_engine.__file__}")
    print(f"module contents: {dir(chess_engine)}")
except ImportError as e:
    print(f"failed to import chess_engine module: {e}")
    import traceback
    traceback.print_exc()
except Exception as e:
    print(f"error during import: {e}")
    import traceback
    traceback.print_exc()
