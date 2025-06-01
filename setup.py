from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "chess_engine",
        ["python_gui/engine_binding.cpp", 
         "src/chess/piece.cpp",
         "src/chess/board.cpp",
         "src/chess/board_moves.cpp", 
         "src/chess/engine.cpp",
         "src/chess/evaluate.cpp"],
        include_dirs=["include"],
        extra_compile_args=["-std=c++17"],
    ),
]

setup(
    name="chess_engine",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
