#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../include/chess/engine.h"
#include "../include/chess/board.h"
#include "../include/chess/evaluate.h"

namespace py = pybind11;

// wrapper function to get the best move as an algebraic string
std::string get_engine_move(const std::string& fen, int depth) {
    chess::Board board(fen);
    chess::Engine engine(depth);
    chess::Move best_move = engine.getBestMove(board);
    return best_move.toAlgebraic();
}

// wrapper function to get the evaluation of a position
int evaluate_position(const std::string& fen) {
    chess::Board board(fen);
    chess::Evaluator evaluator;
    return evaluator.evaluate(board);
}

PYBIND11_MODULE(chess_engine, m) {
    m.doc() = "BrothFish chess engine C++ binding - simplified version";
    
    // wrapper function to get the best move as a string
    m.def("get_best_move", &get_engine_move, 
          "get the best move for a position in FEN notation",
          py::arg("fen"), py::arg("depth") = 3);
    
    // wrapper function to evaluate a position
    m.def("evaluate_position", &evaluate_position,
          "evaluate a position in FEN notation",
          py::arg("fen"));
}
