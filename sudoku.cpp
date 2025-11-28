#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
hello what os up

// our main grid and a copy to remember which cells were originally filled
int grid[9][9];
int origGrid[9][9];
int pts = 0;  // player score

// function prototypes
bool isValidMove(int row, int col, int num);
void fillGrid();
void removeCells();
void print_sudoku_board();
bool isComplete();

int main() {
    srand(time(0));//for unique puzzles each time
    
    cout << "\n================================\n";
    cout << "   WELCOME TO SUDOKU   \n";
    cout << "================================\n";
    
    //makin a complete grid
    fillGrid();
    removeCells(); //removing 30 elements 
    
    //can't change original numbers
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            origGrid[i][j] = grid[i][j];
        }
    }
    
    int row, col, num;
    
    //runs until sudoku is solved
    while (true) {
        print_sudoku_board();
        cout << "\nScore: " << pts << endl;
        
        // check if player finished
        if (isComplete()) {
            cout << "\n*** CONGRATULATIONS! YOU WIN! ***\n";
            cout << "Final Score: " << pts << endl;
            break;
        }
        
        // get player input
        cout << "\nEnter row (1-9): ";
        cin >> row;
        cout << "Enter col (1-9): ";
        cin >> col;
        cout << "Enter number (1-9): ";
        cin >> num;
        
        row--; col--;  //validating user input
        
        //handling invalid input
        if (row < 0 || row > 8 || col < 0 || col > 8 || num < 1 || num > 9) {
            cout << "Invalid input! Try again.\n";
            continue;
        }
        if (origGrid[row][col] != 0) {
            cout << "Can't change that cell!\n";
            continue;
        }
        //checks if move is valid
        if (isValidMove(row, col, num)) {
            grid[row][col] = num;
            pts += 10;
            cout << "Correct! +10 points\n";
        } else {
            cout << "Invalid move! -5 points\n";
            pts -= 5;
        }
    }
    
    return 0;
}

// function definitions

/*
    checks if putting 'num' at grid[row][col] is allowed
    returns false if the same number exists in row, column, or 3x3 box
*/
bool isValidMove(int row, int col, int num) {
    //looking through row
    for (int j = 0; j < 9; j++) {
        if (grid[row][j] == num) 
            return false;
    }
    
    //looking through column
    for (int i = 0; i < 9; i++) {
        if (grid[i][col] == num) 
            return false;
    }
    
    //figure out which 3x3 box we're in and check it
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (grid[i][j] == num) return false;
        }
    }
    
    //if we reach here then move is valid
    return true;
}

//hardcoded complete sudoku grid
void fillGrid() {
    int puzzle[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
    
    //copy to grid
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            grid[i][j] = puzzle[i][j];
        }
    }
}

//removes 30 random cells to create the puzzle
void removeCells() {
    int removed = 0;
    while (removed < 30) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            removed++;
        }
    }
}

//displays grid
void print_sudoku_board() {
    cout << "\n-------------------------\n";
    for (int i = 0; i < 9; i++) {
        cout << "| ";
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j] << " ";
            if ((j + 1) % 3 == 0) cout << "| ";  //vertical divider
        }
        cout << endl;
        if ((i + 1) % 3 == 0) cout << "-------------------------\n";  // horizontal divider
    }
}

//just checks if there are any empty cells left
bool isComplete() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) return false;
        }
    }
    return true;
}
