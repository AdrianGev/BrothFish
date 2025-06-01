cmake_minimum_required(VERSION 3.10)
project(BrothFishPythonBinding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Set pybind11 path directly
set(pybind11_DIR "/Library/Frameworks/Python.framework/Versions/3.12/lib/python3.12/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../include
)

# Add the pybind11 module
pybind11_add_module(chess_engine 
    engine_binding.cpp
    ${CMAKE_SOURCE_DIR}/../src/chess/engine.cpp
    ${CMAKE_SOURCE_DIR}/../src/chess/board.cpp
    ${CMAKE_SOURCE_DIR}/../src/chess/board_moves.cpp
    ${CMAKE_SOURCE_DIR}/../src/chess/piece.cpp
)

# Install the module
install(TARGETS chess_engine DESTINATION ${CMAKE_SOURCE_DIR})
