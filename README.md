# Sudoku Solver

## Build Instructions

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 4
```
The name of the resulting executable is `sudoku_solver`.

If desired, unit tests can be run with the command: `cmake --build . --target test`

## Running Instructions

The program takes two arguments: the search strategy, and the path to an input file.
The search strategy must be either `--simple` or `--smart`.
The format for the input file is described below.

The program does accept a `--help` option to explain its usage.

## Input File Format

Input files must take the form of 81 characters, separated by whitespace.
Empty spaces are represented with `_`,
and assigned cells are represented by their numerical value.
File extension is not checked by the program.

Below is the "easy" problem from the homework document in the appropriate format.

The directory `inputs` contains the "easy," "medium," "hard," and "evil"
puzzles from the homework document,
as well as "trivial" and "already_solved" puzzles,
along with an extra example.

```
_ 3 _ _ 8 _ _ _ 6
5 _ _ 2 9 4 7 1 _
_ _ _ 3 _ _ 5 _ _
_ _ 5 _ 1 _ 8 _ 4
4 2 _ 8 _ 5 _ 3 9
1 _ 8 _ 3 _ 6 _ _
_ _ 3 _ _ 7 _ _ _
_ 4 1 6 5 3 _ _ 2
2 _ _ _ 4 _ _ 6 _
```
