from chess_position import Position, Move
from chess_piece import PieceType, Color, Piece
from chess_board import Board
import random
import time
import os
import sys

# add parent directory to Python path to find the chess_engine module
sys.path.insert(0, os.path.abspath('..'))

# flag to indicate if the C++ engine is available
ENGINE_AVAILABLE = False

# try to import the C++ engine module
try:
    import chess_engine
    print("C++ chess engine module loaded successfully")
    ENGINE_AVAILABLE = True
except ImportError as e:
    print(f"Warning: C++ chess engine module not found: {e}. Using Python-only mode.")
    chess_engine = None

class EngineWrapper:
    # wrapper for the C++ chess engine
    
    def __init__(self, depth=3):
        # init the engine with the specified search depth
        self.depth = depth
        self.nodes_searched = 0
        print(f"engine initialized with depth {depth}")
        if ENGINE_AVAILABLE:
            print("C++ chess engine is available")
        else:
            print("Running in Python-only mode (no C++ engine)")
    
    def set_depth(self, depth):
        # set the search depth
        self.depth = depth
        print(f"engine depth set to {depth}")
    
    def get_best_move(self, board):
        # get the best move for the current position
        start_time = time.time()
        
        # if C++ engine is not available, use random move selection
        if not ENGINE_AVAILABLE:
            print("C++ engine not available, returning random legal move")
            moves = self._get_legal_moves_python(board)
            move = random.choice(moves) if moves else None
            elapsed = time.time() - start_time
            print(f"engine move: {move} (in {elapsed:.2f} seconds, 0 nodes)")
            return move
        
        try:
            # get the FEN representation of the board
            fen = board.to_fen()
            
            # use the C++ engine to get the best move as algebraic notation
            start_time = time.time()
            move_str = chess_engine.get_best_move(fen, self.depth)
            elapsed = time.time() - start_time
            
            # parse the algebraic notation into a Move object
            from_pos = Position.from_algebraic(move_str[:2])
            to_pos = Position.from_algebraic(move_str[2:4])
            
            # handle promotion if present
            promotion = None
            if len(move_str) > 4:
                promotion_map = {
                    'q': PieceType.QUEEN,
                    'r': PieceType.ROOK,
                    'b': PieceType.BISHOP,
                    'n': PieceType.KNIGHT
                }
                promotion = promotion_map.get(move_str[4].lower())
            
            # create the move object
            move = Move(from_pos, to_pos, promotion)
            
            # estimate nodes searched (we don't have direct access in simplified binding)
            self.nodes_searched = int(1000 * self.depth * elapsed)  # Rough estimate
            
            print(f"engine move: {move} (in {elapsed:.2f} seconds, ~{self.nodes_searched} nodes)")
            return move
            
        except Exception as e:
            print(f"error using C++ engine: {e}. Falling back to random move.")
            moves = self._get_legal_moves_python(board)
            move = random.choice(moves) if moves else None
            elapsed = time.time() - start_time
            print(f"engine move: {move} (in {elapsed:.2f} seconds, 0 nodes)")
            return move
    
    def get_nodes_searched(self):
        # get the number of nodes searched in the last search
        return self.nodes_searched
    
    def _get_legal_moves_python(self, board):
        # get legal moves using Python implementation (fallback)
        from chess_move_generator import MoveGenerator
        generator = MoveGenerator(board)
        return generator.generate_legal_moves()
