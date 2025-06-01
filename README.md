# BrothFish

A chess engine implemented in C++ with a Python GUI. The engine uses a minimax algorithm with alpha-beta pruning for move evaluation.

## Project Structure

- `include/` - C++ header files
- `src/` - C++ source files
- `python_gui/` - Python GUI for the chess engine
- `build/` - Build directory (created by CMake)

## Building the C++ Engine

This project uses CMake as its build system. To build the C++ engine:

```bash
# Configure the project
cmake -B build -S .

# Build the project
cmake --build build
```

The executable will be placed in the `build/bin` directory.

## Building the Python Bindings

The project includes Python bindings using pybind11. To build the Python module:

```bash
# Create and activate a virtual environment (recommended)
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install required packages
pip install -r requirements.txt

# Build the Python module
python setup.py build_ext --inplace
```

## Running the Chess GUI

After building the Python module, you can run the chess GUI:

```bash
# Make sure the virtual environment is activated
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Run the GUI from the project root directory
cd python_gui
python main.py
```

## Running the C++ Application

You can also run the standalone C++ application:

```bash
./build/bin/BrothFish
```

## Testing

To test the Python bindings:

```bash
# From the project root
python debug_test.py

# Or from the python_gui directory
cd python_gui
python test_simple_engine.py
```

## Development

This project is set up with VSCode configuration files for easy development:

- Press F5 to build and debug the application
- Use the CMake Build task to build without debugging

## General Development Workflow

When making changes to the C++ code, follow this workflow to recompile and test your changes:

```bash
# 1. Activate the virtual environment
source venv/bin/activate  # On Windows: venv\Scripts\activate

# 2. Recompile the Python module with your changes
python setup.py build_ext --inplace

# 3. Run a quick test to verify your changes
python debug_test.py

# 4. Run the GUI to test in a real game
cd python_gui
python main.py
```

This workflow ensures that your C++ changes are properly compiled into the Python module and can be tested quickly.

## Engine Features

- Minimax algorithm with alpha-beta pruning
- Material evaluation
- Center control evaluation
- Placeholder for additional evaluation components:
  - Piece position tables
  - Mobility evaluation
  - Pawn structure evaluation
  - King safety evaluation

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher
- Python 3.8 or higher
- pybind11
