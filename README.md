# Sudoku-Game
## Team-info

### Group Members and their roles:

* #### Muhammad Musa Bin Mansha -> Programmer, Documenter, Tester
* #### Kareem Azzam -> Programmer, Flowchart designer, Ui enhancements

## Overview

This is a C++ console-based Sudoku game.
It generates a fully solved Sudoku grid, randomizes it, removes cells according to difficulty, and ensures the puzzle has a **unique solution** using forward and backward backtracking solvers.

The player can fill cells, request hints, check for errors, auto-solve the puzzle, and track points.

The program uses Windows console colors (`windows.h`) for display, so it is kind of windows OS dependent.

---

## Features

* Randomized valid Sudoku puzzle generation
* Unique-solution verification using dual solving(backword and forward)
* Difficulty selection (Easy / Medium / Hard)
* Hint system
* Error checking for wrong inputs
* Score tracking
* Automatically solves on command
* Colored board output
* Timer start and statistics

---

## File Structure

The entire program is contained in one `.cpp` file containing:

* Global 9x9 grids
* Puzzle generator
* Solver (forward & backward)
* Unique-solution checker
* Player menu
* Input handlers
* Board printer
* Hint system
* Error detection

---

## Variables

| Name               | Description                                                  |
| ------------------ | ------------------------------------------------------------ |
| `grid[9][9]`       | The current working grid the player interacts with           |
| `origGrid[9][9]`   | Stores original clues; prevents editing fixed cells          |
| `solvedGrid[9][9]` | Stores the full valid solution                               |
| `gridfw`, `gridbk` | Copies used for forward/backward solving to check uniqueness |
| `pts`              | Player’s score                                               |
| `startTime`        | Tracks when the game started                                 |

---

## Important Functions

### Move Validation

* **`bool isValidMove(int row, int col, int num)`**
  Checks row, column, and 3×3 box(subgrid).

* **`bool isComplete()`**
  Returns true if no zeros remain.

---

### Solving Algorithms

* **`bool solveGrid()`**
  Standard backtracking (1 → 9).

* **`bool solveBackward()`**
  Reverse backtracking (9 → 1).

* **`bool UniqueSol()`**
  Runs both solvers on copied grids and compares results.
  If both solutions match → puzzle has a unique solution.(99 percent chance)

---

### Puzzle Generation

* **`void fillGrid()`**
  Loads a complete, valid Sudoku solution.

* **`void randomizeGrid()`**
  Applies swaps on rows, columns, and digits.

* **`void saveSolution()`**
  Stores solved grid.

* **`void removeCells(int level)`**
  Removes numbers according to difficulty.
  Re-runs uniqueness check until valid.

* **`void saveOriginal()`**
  Marks original puzzle cells so player cannot edit them.

---

### User Interface / Gameplay

* **`void print_sudoku_board()`**
  Prints the current board with color formatting.

* **`void handleFillCell()`**
  Player enters row, column, and value.

* **`void getHint()`**
  Reveals the correct value of the first empty cell.

* **`void showErrors()`**
  Highlights rule violations.

* **`void handleAutoSolve()`**
  Immediately solves the puzzle.

* **`void showMenu()`**
  Displays main menu.

* **`void gameLoop()`**
  Main game controller.

---

## Compilation

### Requirements

* Windows OS
* g++ compiler (MinGW recommended)

### Compile

```bash
g++ sudoku.cpp -o sudoku
```

### Run

```bash
./sudoku
```

---

## Inputs

* Difficulty selection (numbers 1–3)
* For filling cells:

  * Row (1–9)
  * Column (1–9)
  * Number (1–9 or 0 to clear)

---

## Outputs

* Colored Sudoku board
* Messages such as:

  * Correct (+10 points)
  * Wrong placement(-5 points)
  * Hint output(-10 points)
  * Current grid satisfies Sudoku rules
  * Congrats message if correctly solved

---

## AI tool Reflection

### Tool Used: ChatGPT
### Reflection
* Refined Readme.md file structure.
* Gave created grid after removing cells to chatgpt to calculate the probability of
  finding a unique solution of the grid made after removing cells. This saved our time by saving us from solving the puzzle
  multiple times to see if there are multiple solutions.
* for debugging the solver function

  ---

## Future Additions 

* More advanced randomization
* More advanced uniqueness checker
* GUI (SFML) for improved Ui and experience
* Leaderboard or scoring database

---
