import sys
import os
import traceback

# test script incase it crashes out

print(f"python ver: {sys.version}")
print(f"current working directory: {os.getcwd()}")
print(f"python path: {sys.path}")
print(f"files in current directory: {os.listdir('.')}")

try:
    import chess_engine
    print("successfully imported chess_engine module")
    print(f"module path: {chess_engine.__file__}")
    print(f"available attributes: {dir(chess_engine)}")
    
    # test creating some objects
    print("\ntesting module functionality:")
    try:
        board = chess_engine.CppBoard()
        print("- created CppBoard successfully")
    except Exception as e:
        print(f"- failed to create CppBoard: {e}")
        traceback.print_exc()
    
    try:
        engine = chess_engine.CppEngine(3)
        print("- created CppEngine successfully")
    except Exception as e:
        print(f"- failed to create CppEngine: {e}")
        traceback.print_exc()
        
except ImportError as e:
    print(f"failed to import chess_engine module: {e}")
    traceback.print_exc()
except Exception as e:
    print(f"unexpected error: {e}")
    traceback.print_exc()
