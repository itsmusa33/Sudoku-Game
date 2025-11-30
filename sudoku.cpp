#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;

//our main grid and a copy to remember which cells were originally filled
int grid[9][9];
int origGrid[9][9];
int solvedGrid[9][9];
int gridfw[9][9];  //for forward solution
int gridbk[9][9];  //for backward solution
int pts = 0;  //player score
long startTime;

//function prototypes
void clr(int c);
void rst();
bool isValidMove(int row, int col, int num);
bool solveForward();
bool solveBackward();
bool hasUniqueSolution();
bool solveGrid();
void handleAutoSolve();
void fillGrid();
void randomizeGrid();
void removeCells(int level);
int selectDifficulty();
int calculateTimeBonus();
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
    startTime = time(0);  // start timer
    
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
//solver that tries each num from 1 to 9 and tries with diff num if it gets stuck
bool solveGrid(){
    for (int row = 0; row < 9; row++){
        for (int col = 0; col < 9; col++){
            if (grid[row][col] == 0){//lookin for empty cell
                for (int num = 1; num <= 9; num++){//trying nums 1 to 9
                    if (isValidMove(row, col, num)) {
                        grid[row][col] = num;  //placing number
                        if (solveGrid()) 
                            return true;  //keep going
                        grid[row][col] = 0;  //if doesnt work 
                    }
                 }
                return false;  //if no num works
             }
       }
    }
    return true;  //when there are no empty cells it is solved
}
bool solveBackward(){//same as solvegrid but tries from 9 to 1(opposite order)
    for (int row = 0; row < 9; row++){
        for (int col = 0; col < 9; col++){
            if (grid[row][col] == 0){
                for (int num = 9; num >= 1; num--){//9 to 1
                    if (isValidMove(row, col, num)){
                        grid[row][col] = num;
                        if (solveBackward())
                            return true;
                        grid[row][col] = 0;
                     }
                }
                return false;
         }
        }
    }
    return true;
}
//this checks for unique solution by solving in 2 ways if they are same then its unique
bool UniqueSol(){
    int backup[9][9];//for saving grid
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            backup[i][j] = grid[i][j];
    //solve for 1 to 9(forward)
    solveGrid();
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            gridfw[i][j] = grid[i][j];
    //make the grid same as before
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = backup[i][j];
    
    //solve going backward (9-1) and then save the result
    solveBackward();
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            gridbk[i][j] = grid[i][j];
    
    //restore the grid again
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            grid[i][j] = backup[i][j];
    
   //comparing solutions
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (gridfw[i][j] != gridbk[i][j])
                return false;  //different means multiple solutions
    
    return true;  //same means unique sol
}
//auto-solve function costing 20 points
void handleAutoSolve(){
    pts -= 20; //minus score
    
    int elapsed = time(0) - startTime;
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    
    system("cls");
    clr(96);
    cout << "\n=== SOLVING... ===\n";
    rst();
    Sleep(4000);
    
    solveGrid(); //solvin it
    
    system("cls");
    clr(32);
    cout << "\n=== SOLVED! ===\n";
    rst();
    print_sudoku_board();
    
    cout << "\nTime: " << mins << ":";
    if (secs < 10) {
        cout << "0";
    }
    cout << secs << endl;
    cout << "Final Score: " << pts << endl;
    cout << "\nThanks for playing!\n";
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
// removes cells to create the puzzle
// keeps trying until we get a puzzle with exactly one solution
void removeCells(int level){
    int toRemove = 26;  //easy
    if (level == 2) 
        toRemove = 36;  //medium
    else if (level == 3)
        toRemove = 42;  //hard
    
    cout << "Generating puzzle...\n";
    //keep trying until we get a puzzle with a unique sol
    do {
        //reset grid to the solved state
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                grid[i][j] = solvedGrid[i][j];
        int removed = 0; //randomly remove cells
        while (removed < toRemove){
            int r = rand() % 9;
            int c = rand() % 9;
            if (grid[r][c] != 0){
                grid[r][c] = 0;
                removed++;
               }
         }
    } while (!UniqueSol());  //keep trying if not unique
    
    cout << "Puzzle ready!\n";
    Sleep(1000);
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
        cout << "1. Easy \n";
        cout << "2. Medium \n";
        cout << "3. Hard \n";
        cout << "Enter choice (1-3): ";
        cin >> difficulty;
        
        if (difficulty >= 1 && difficulty <= 3) {
            return difficulty;
           }
        cout << "Invalid choice! Please enter 1, 2, or 3.\n";
     }
}

// bonus points for finishing fast
int calculateTimeBonus() {
    int elapsed = time(0) - startTime;
    if (elapsed < 120) 
        return 50;   // under 2 min
    if (elapsed < 300)
        return 30;   // under 5 min
    if (elapsed < 600) 
        return 10;   // under 10 min
    return 0;
}
//show menu options
void showMenu() {
    cout << "\n1. Fill cell\n";
    cout << "2. Check errors\n";
    cout << "3. Get hint (-10 pts)\n";
    cout << "4. Auto-solve (-20 pts)\n";
    cout << "5. Quit\n";
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
void gameLoop() 
{
    int choice;
    while (true) {
        system("cls");
        int seconds = time(0) - startTime;
int mins = seconds / 60;
int secs = seconds % 60;

cout << "\n=== SUDOKU === Score: " << pts << " | Time: " << mins << ":";
if (secs < 10)
{
    cout << "0";
}
cout << secs << "\n";

        print_sudoku_board();
       if (isComplete()) 
       {
    int bonus = calculateTimeBonus();
    pts += bonus;
    
    int elapsed = time(0) - startTime;
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    
    clr(32);
    cout << "\n*** CONGRATULATIONS! YOU WIN! ***\n";
    rst();
    
    cout << "Time: " << mins << ":";
    if (secs < 10) {
        cout << "0";
    }
    cout << secs << endl;
    
    if (bonus > 0) {
        clr(10);
        cout << "Time Bonus: +" << bonus << " points!\n";
        rst();
    }
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
            handleAutoSolve();
            Sleep(2000);
            break;
        }
        else if (choice == 5) {
            cout << "Thanks for playing!\n";
            break;
        }
    }
}
