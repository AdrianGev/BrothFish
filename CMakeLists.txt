cmake_minimum_required(VERSION 3.10)
project(BrothFish VERSION 1.0)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# find pybind11
find_package(pybind11 REQUIRED)
# include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
# source files for the main executable
set(SOURCES
    src/chess/piece.cpp
    src/chess/board.cpp
    src/chess/engine.cpp
    main.cpp
)
# source files for the Python module
set(MODULE_SOURCES
    src/chess/piece.cpp
    src/chess/board.cpp
    src/chess/engine.cpp
    python_gui/engine_binding.cpp
)
# add executable
add_executable(BrothFish ${SOURCES})
# add Python module
pybind11_add_module(chess_engine ${MODULE_SOURCES})
# output binary to bin directory
set_target_properties(BrothFish PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# output Python module to python_gui directory
set_target_properties(chess_engine PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/python_gui
)
# create a directory for resources
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/resources/images)
# copy chess pieces to binary directory
file(GLOB CHESS_PIECES ${PROJECT_SOURCE_DIR}/resources/images/pieces-basic-png/*.png)
foreach(PIECE ${CHESS_PIECES})
    file(COPY ${PIECE} DESTINATION ${CMAKE_BINARY_DIR}/bin/resources/images/pieces-basic-png/)
endforeach()