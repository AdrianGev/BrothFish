#!/usr/bin/env python3

# test script to diagnose module import issues in case it crashes out

import sys
import os

print(f"Python version: {sys.version}")
print(f"Current directory: {os.getcwd()}")
print(f"Python path: {sys.path}")
print(f"Files in current directory: {os.listdir('.')}")

try:
    import chess_engine
    print(f"chess_engine module found at: {chess_engine.__file__}")
    print(f"chess_engine module contents: {dir(chess_engine)}")
except ImportError as e:
    print(f"Import error: {e}")
except Exception as e:
    print(f"Other error: {e}")
    import traceback
    traceback.print_exc()
