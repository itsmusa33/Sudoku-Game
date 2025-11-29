#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

//our main grid and a copy to remember which cells were originally filled
int grid[9][9];
int origGrid[9][9];
int solvedGrid[9][9];
int pts = 0;  // player score

//function prototypes
void clr(int c);
void rst();
bool isValidMove(int row, int col, int num);
void fillGrid();
void randomizeGrid();
void removeCells(int level);
int selectDifficulty();
void showMenu();
void showErrors();
void getHint();
void print_sudoku_board();
bool isComplete();
void saveSolution();
void saveOriginal();
void handleFillCell();
void gameLoop();


int main() {
    srand(time(0));//for unique puzzles each time
    
    cout << "\n================================\n";
    cout << "        WELCOME TO SUDOKU   \n";
    cout << "================================\n";
     int difficulty = selectDifficulty();
    fillGrid();
    randomizeGrid();
    saveSolution();
    removeCells(difficulty);
    saveOriginal();
    
    gameLoop();
    
    return 0;
}
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
// set console color
void clr(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// reset to default color
void rst() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void randomizeGrid(){
  //shuffle rows within each block of 3 rows
for (int block = 0; block < 3; block++){
    int row1 = block * 3 + rand() % 3;//Pick two random rows from the same 3 row block
    int row2 = block * 3 + rand() % 3;
    for (int j = 0; j < 9; j++) { //Swappin the two rows column by column
        int temp = grid[row1][j];
        grid[row1][j] = grid[row2][j];
        grid[row2][j] = temp;
       }
}
//shuffle columns within each block of 3 columns
for (int block = 0; block < 3; block++){
    int col1 = block * 3 + rand() % 3;//same as in previous but for columns
    int col2 = block * 3 + rand() % 3;
    for (int i = 0; i < 9; i++) {
        int temp = grid[i][col1];
        grid[i][col1] = grid[i][col2];
        grid[i][col2] = temp;
      }
}

    // swap two random numbers for more variety
int num1 = (rand() % 9) + 1;
int num2 = (rand() % 9) + 1;
for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
        if (grid[i][j] == num1) grid[i][j] = num2;
        else if (grid[i][j] == num2) grid[i][j] = num1;
    }
}
}

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
void removeCells(int level) {
    int Remove = 30;
    if (level == 2)
        Remove = 40;
    else if (level == 3) 
        Remove = 50;
    int removed = 0;
    while (removed < Remove) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (grid[row][col] != 0) {
            grid[row][col] = 0;
            removed++;
        }
    }
}
void saveSolution(){ //save the solved grid before removing cells
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            solvedGrid[i][j] = grid[i][j];
        }
    }
}
void saveOriginal() { //save original grid to track which cells can be changed
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            origGrid[i][j] = grid[i][j];
        }
    }
}

//displays grid
void print_sudoku_board() {
    cout << "\n    1 2 3   4 5 6   7 8 9\n";
    cout << "  -------------------------\n";
    for (int i = 0; i < 9; i++) {
        cout << i+1 << " | ";
        for (int j = 0; j < 9; j++) {
            int val = grid[i][j];
            
            if (val == 0) {
                clr(96);  // yellow for empty
                cout << "0";
                rst();
            }
            else if (origGrid[i][j] == 0 && val != 0) {
                // player-filled cell
                if (val == solvedGrid[i][j]) clr(32);  // green = correct
                else clr(64);  // red = wrong
                cout << val;
                rst();
            }
            else {
                cout << val;  // original cell, no color
            }
            cout << " ";
            if ((j + 1) % 3 == 0) cout << "| ";
        }
        cout << endl;
        if ((i + 1) % 3 == 0) cout << "  -------------------------\n";
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
//Handles user input for filling or clearing a cell on the board
void handleFillCell() {
    int row, col, num;
    
    cout << "Enter row (1-9): ";
    cin >> row;
    cout << "Enter col (1-9): ";
    cin >> col;
    cout << "Enter number (1-9, or 0 to clear): ";
    cin >> num;

    row--; col--;//Convert to 0 based indexing for array access
    //Basic safety checks for invalid input
    if (row < 0 || row > 8 || col < 0 || col > 8 || num < 0 || num > 9) {
        cout << "Invalid input!\n";
        return;
    }
    if (origGrid[row][col] != 0) {
        cout << "Can't change that cell!\n";
        return;
    }
    
    if (num == 0) {
        grid[row][col] = 0;
        cout << "Cell cleared.\n";
    }
    else {
        grid[row][col] = num;//Fill the selected cell
        if (num == solvedGrid[row][col]) 
        { //gives points if entered num is correct
           clr(32);
            cout << "Correct! +10 points\n";
            rst();
            pts += 10;
        } else {
           clr(64);
            cout << "Wrong! -5 points\n";
            rst();
            pts -= 5;
        }
    }
Sleep(1500);
}
//select difficulty at start
int selectDifficulty(){
    int difficulty;
    while (true) {
        cout << "\nSelect difficulty:\n";
        cout << "1. Easy (30 empty cells)\n";
        cout << "2. Medium (40 empty cells)\n";
        cout << "3. Hard (50 empty cells)\n";
        cout << "Enter choice (1-3): ";
        cin >> difficulty;
        
        if (difficulty >= 1 && difficulty <= 3) {
            return difficulty;
           }
        cout << "Invalid choice! Please enter 1, 2, or 3.\n";
     }
}
//show menu options
void showMenu() {
    cout << "\n1. Fill cell\n";
    cout << "2. Check errors\n";
    cout << "3. Get hint (-10 pts)\n";
    cout << "4. Quit\n";
    cout << "Enter choice: ";
}
//check if grid satisfies sudoku rules
void showErrors() {
    bool hasErrors = false;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] != 0) { //Only check cells that actually have a number
                int temp = grid[i][j];//store the number
                grid[i][j] = 0;//clear cell for check
                if (!isValidMove(i, j, temp)) {//returns error if number cannot be placed
                    hasErrors = true;
                   }//putting back after checking
                grid[i][j] = temp;
          }
        }
    }
    
    if (hasErrors) {
        clr(64);
        cout << "Current grid does NOT satisfy Sudoku rules.\n";
        rst();
    } else {
        clr(32);
        cout << "Current grid satisfies Sudoku rules.\n";
        rst();
    }
}
//give hint for first empty cell
void getHint() {
    for (int i = 0; i < 9; i++) {//going through each cell
        for (int j = 0; j < 9; j++){
            if (grid[i][j] == 0) {//goes in if the cell is empty
                cout << "Hint: Row " << i+1 << ", Col " << j+1;//giving player correct num for the cell
                cout << " -> " << solvedGrid[i][j] << endl;//uses solved grid for hints
                pts -= 10;
                clr(64);
                cout << "(-10 points)\n";
                rst();
                return;
               }
         }
    }
    cout << "No empty cells to hint!\n";
}
//draws the board,checks win condition.handle moves
// main game loop with menu
void gameLoop() {
    int choice;
    while (true) {
        system("cls");
        cout << "\n=== SUDOKU === Score: " << pts << "\n";
        print_sudoku_board();
        if (isComplete()) {
            cout << "\n*** CONGRATULATIONS! YOU WIN! ***\n";
            cout << "Final Score: " << pts << endl;
            break;
        }
        showMenu();
        cin >> choice; 
        if (choice == 1) {
            handleFillCell();
        }
        else if (choice == 2) {
            showErrors();
            Sleep(3500);
        }
        else if (choice == 3) {
            getHint();
            Sleep(5000);
        }
        else if (choice == 4) {
            cout << "Thanks for playing!\n";
            break;
        }
    }
}
