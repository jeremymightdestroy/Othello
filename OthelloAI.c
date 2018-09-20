/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   OthelloAI.c
 * Author: leejere7
 *
 * Created on March 9, 2017, 1:05 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h> 

// Initializes with starting 4 pieces in the middle. If n is not even, prints 4 pieces a single increment up and left to the center. 
void initializeBoard(char board[][26], int n) {
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if((i == ((n / 2) - 1)) && (j == ((n / 2) - 1))) {
                board[i][j] = 'W';
            }
            else if((i == ((n / 2) - 1)) && (j == (n / 2))) {
                board[i][j] = 'B';
            }
            else if((i == (n / 2)) && (j == ((n / 2) - 1))) {
                board[i][j] = 'B';
            }
            else if((i == (n / 2)) && (j == (n / 2))) {
                board[i][j] = 'W';
            }
            else{
                board[i][j] = 'U'; 
            }
        }
    }        
    return;
}

// Prints a n x n board with recorded values. 
void printBoard(char board[][26], int n) {
    
    const char grid[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    
    printf("  ");
    for(int i = 0; i < n; i++) {
        printf("%c", grid[i]);
    }
    for(int i = 0; i < n; i++) {
        printf("\n");
        printf("%c ", grid[i]);
        for(int j = 0; j < n; j++) {
                printf("%c", board[i][j]);
        }
    }        
    printf("\n");
    return;
}

// Records single piece of user-inputted color where entered row and entered column intersect.
void placePiece(char board[][26], char row, char col, char currentColour) {
   
    int numberRow = abs('a' - row);
    int numberColumn = abs('a' - col);
    board[numberRow][numberColumn] = currentColour;
    return;
}

// Checks whether given piece lies within the n x n board.
bool positionInBounds(char board[][26], int n, char row, char col) {
    
    if((row < n) && (row >= 0) && (col < n) && (col >= 0)) {
        return(true);
    }
    else{
        return(false);
    }
}

// Continuously checks for opposite color in the given direction. Reports true if line terminates with player color. If not, returns false. 
bool checkLegalDirection(char board[][26], int n, char row, char col, char currentColour, int deltaRow, int deltaCol) {
    
    char opponentColour;
    int count = 0;
    int numberRow = abs('a' - row);
    int numberColumn = abs('a' - col);
    int counterRow = numberRow + deltaRow;
    int counterCol = numberColumn + deltaCol;
    
    // Determines opponent color.
    if (currentColour == 'W') {
        opponentColour = 'B';
    }
    else {
        opponentColour = 'W';
    }
    
    // Continously checks for opposite color. Only reports true if line terminates with player color and an opposite color exists in that line.  
    while((positionInBounds(board, n, counterRow, counterCol)) && (board[counterRow][counterCol] == opponentColour)) {
        count = count + 1;
        counterRow = counterRow + deltaRow;
        counterCol = counterCol + deltaCol;
    }
    if(!positionInBounds(board, n, counterRow, counterCol)) {
        return(false);
    }
    else if(board[counterRow][counterCol] == 'U') {
        return(false);
    } 
    else if(count == 0) {
        return(false);
    }
    else {
        return(true);
    }
}

// Sets the color of current turn depending on turn
void setPlayerTurn(int turnCount, char computerColour, char* currentColour) {
   if((turnCount % 2) == 0) {
        *currentColour = 'W';
    }
    else if((turnCount % 2) == 1) {
        *currentColour = 'B';
    }
    return;
}

// Records move of user or computer.
void recordMove(char board[][26], int boardSize, char currentColour, char* row, char* col, char computerColour) {
    if((currentColour == 'W') && (computerColour == 'W')) {
        computerCode(board, boardSize, &*row, &*col, computerColour);
    }
    else if((currentColour == 'W') && (computerColour == 'B')) {
        printf("Enter move for colour W (RowCol): ");
        scanf(" %c%c", row, col);
    }
    else if((currentColour == 'B') && (computerColour == 'B')) {
        computerCode(board, boardSize, &*row, &*col, computerColour);
    }
    else if((currentColour == 'B') && (computerColour == 'W')) {
        printf("Enter move for colour B (RowCol): ");
        scanf(" %c%c", row, col);
    }  
    return;
}

// Checks if current colour can be put in U. If not, returns false.
bool validMovesLeft(char board[][26], int boardSize, char row, char col, char currentColour) {
    int emptySpaceCount = 0;
    
    for(int i = 0; i < boardSize; i++) {
        for(int j = 0; j < boardSize; j++) {
            if(board[i][j] == 'U') {
                bool notCountedMoreThanOnce = false;
                char emptySpaceRow = ('a' + i);
                char emptySpaceColumn = ('a' + j);
                for(int k = -1; k <= 1; k++) {
                    for(int l = -1; l <= 1; l++) {
                        if ((checkLegalDirection(board, boardSize, emptySpaceRow, emptySpaceColumn, currentColour, k, l)) && (notCountedMoreThanOnce == false)) {
                            emptySpaceCount = emptySpaceCount + 1;
                            notCountedMoreThanOnce = true;
                        }      
                    }
                }         
            }
        }
    }
   if(emptySpaceCount > 0) {
       return(true);
   } 
   else {
       return(false);
   }
}

// Checks whether move user/computer does is valid.
bool moveChecker(char board[][26], int boardSize, char row, char col, char currentColour) {
    int numberRow = abs('a' - row);
    int numberCol = abs('a' - col);
    bool moveConfirmation = false;
    
    if((positionInBounds(board, boardSize, numberRow, numberCol)) && (board[numberRow][numberCol] == 'U')) {
        for(int k = -1; k <= 1; k++) {
            for(int l = -1; l <= 1; l++) {
                if (checkLegalDirection(board, boardSize, row, col, currentColour, k, l)) {
                    moveConfirmation = true;        
                }
            }
        }
    }
    return(moveConfirmation);
}

// FLips tiles of opposite colour in all cardinal directions that end with current player colour.
void oppositeColourFlipper(char board[][26], int boardSize, char row, char col, char currentColour) {
    char oppositeColour;
    int numberRow = abs('a' - row);
    int numberColumn = abs('a' - col);
    int counterRow = numberRow;
    int counterCol = numberColumn;
    
    // Determines opposite color.
    if (currentColour == 'W') {
        oppositeColour = 'B';
    }
    else {
        oppositeColour = 'W';
    }
    
    for(int k = -1; k <= 1; k++) {
        for(int l = -1; l <= 1; l++) {
            if (checkLegalDirection(board, boardSize, row, col, currentColour, k, l)) {
                counterRow = numberRow + k;
                counterCol = numberColumn + l;
                while(board[counterRow][counterCol] == oppositeColour) {
                    board[counterRow][counterCol] = currentColour;
                    counterRow = counterRow + k;
                    counterCol = counterCol + l;
                }
            }
        }
    } 
    return;
}

// Records a move prioritizing in the order of: (1) getting the corners, (2) not giving opponent corners, (3) plays to the advantage of conquered corners, (4) getting control of the sides, (5) flipping maximum amount of tiles.
void computerCode(char board[][26], char boardSize, char* row, char* col, char computerColour) {
    
    double score = 0;
    double highScore = 0;
    char oppositeColour;
    
    // Determines opposite color.
    if (computerColour == 'W') {
        oppositeColour = 'B';
    }
    else {
        oppositeColour = 'W';
    }
    
    // Cycles through the whole board looking for empty spaces that are viable moves. 
    for(int i = 0; i < boardSize; i++) {
        for(int j = 0; j < boardSize; j++) {
            if(board[i][j] == 'U') {
                score = 0;
                bool validEmpty = false;
                for(int k = -1; k <= 1; k++) {
                    for(int l = -1; l <= 1; l++) {
                        int counterRow = i + k;
                        int counterCol = j + l;
                        if(checkLegalDirection(board, boardSize, 'a' + i, 'a' + j, computerColour, k, l) == true) {
                            validEmpty = true;
                            while(board[counterRow][counterCol] == oppositeColour) {
                                
                                // Gives a move a bonus score if it flips opponent side pieces. 
                                if((i == 0) && (counterRow == 0)) {
                                    score = score + 2;
                                }
                                else if((i == (boardSize - 1)) && (counterRow == (boardSize - 1))) {
                                    score = score + 2;
                                }
                                else if((j == 0) && (counterCol == 0)) {
                                    score = score + 2;
                                }
                                else if((j == (boardSize - 1)) && (counterCol == (boardSize - 1))) {
                                    score = score + 2;
                                } 
                                
                                // Gives a rating for amount of tiles a move flips.
                                score = score + .2;
                                
                                // If a move gives the opponent the potential to place a side piece, gives the move a low rating.
                                if((i == 1) && (k == 1) && (l == 0)) {
                                    score = .002;    
                                }
                                else if((i == (boardSize - 2)) && (k == -1) && (l == 0)) {
                                    score = .002;
                                }
                                else if((j == 1) && (k == 0) && (l == 1)) {
                                    score = .002;
                                }
                                else if((j == (boardSize - 2)) && (k == 0) && (l == -1)) {
                                    score = .002;
                                }
                                        
                                counterRow = counterRow + k;
                                counterCol = counterCol + l;
                            }
                        }      
                    }
                }    
                
                if(validEmpty == true) {
                    
                    // Maximum priority on getting the corner.
                    if((i == 0 ) && (j == 0 )) {
                        score = score + 100;
                    }
                    else if((i == (boardSize - 1)) && (j == 0 )) {
                        score = score + 100;
                    }
                    else if((i == 0) && (j == (boardSize - 1))) {
                        score = score + 100;
                    }
                    else if((i == (boardSize - 1)) && (j == (boardSize - 1))) {
                        score = score + 100;
                    }
                    // Bonus score if move is on the side. 
                    if(i == 0) {
                        score = score + .7;
                    }
                    else if(i == (boardSize - 1)) {
                        score = score + .7;
                    }
                    else if(j == 0) {
                        score = score + .7;
                    }
                    else if(j == (boardSize - 1)) {
                        score = score + .7;
                    }
                    
                    // If a move gives the opponent the potential to get a corner, give that move the lowest priority possible.
                    if((board[0][0] == 'U') && (i == 0) && (j == 1)) {
                        score = .001;
                    }
                    else if((board[0][0] == 'U') && (i == 1) && (j == 0)) {
                        score = .001;
                    }
                      else if((board[0][0] == 'U') && (i == 1) && (j == 1)) {
                        score = .001;
                    }
                    else if((board[0][boardSize - 1] == 'U') && (i == 0) && (j == (boardSize - 2))) {
                        score = .001;
                    }
                    else if((board[0][boardSize - 1] == 'U') && (i == 1) && (j == (boardSize - 1))) {
                        score = .001;
                    } 
                    else if((board[0][boardSize - 1] == 'U') && (i == (boardSize - 2)) && (j == (boardSize - 2))) {
                        score = .001;
                    } 
                    else if((board[boardSize - 1][0] == 'U') && (i == (boardSize - 2)) && (j == 0)) {
                        score = .001;
                    }
                    else if((board[boardSize - 1][0] == 'U') && (i == boardSize - 1) && (j == 1)) {
                        score = .001;
                    }
                    else if((board[boardSize - 1][0] == 'U') && (i == boardSize - 2) && (j == 1)) {
                        score = .001;
                    }
                    else if((board[boardSize -1][boardSize - 1] == 'U') && (i == (boardSize - 2)) && (j == (boardSize - 1))) {
                        score = .001;
                    }
                    else if((board[boardSize -1][boardSize - 1] == 'U') && (i == (boardSize - 1)) && (j == (boardSize - 2))) {
                        score = .001;
                    }
                    else if((board[boardSize -1][boardSize - 1] == 'U') && (i == (boardSize - 2)) && (j == (boardSize - 2))) {
                        score = .001;
                    }
                    // If a corner is conquered and a move plays off that advantage, give that move a bonus score. 
                    if(board[0][0] == computerColour) {
                        if(i == 0) {
                            int counterLeft = j - 1;
                            while(board[i][counterLeft] == computerColour) {
                                if(counterLeft == 0) {
                                    score = score + 30;
                                }
                                counterLeft = counterLeft - 1;    
                            }
                        }
                        else if(j == 0) {
                            int counterUp = i - 1;
                            while(board[counterUp][j] == computerColour) {
                                if(counterUp == 0) {
                                    score = score + 30;
                                }
                                counterUp = counterUp - 1;
                            }
                        }
                    }
                    else if(board[0][boardSize - 1] == computerColour) {
                        if(i == 0) {
                            int counterRight = j + 1;
                            while(board[i][counterRight] == computerColour) {
                                if(counterRight == (boardSize - 1)) {
                                    score = score + 10;
                                }
                                counterRight = counterRight + 1;    
                            }
                        }
                        else if(j == (boardSize - 1)) {
                            int counterUp = i - 1;
                            while(board[counterUp][j] == computerColour) {
                                if(counterUp == 0) {
                                    score = score + 30;
                                }
                                counterUp = counterUp - 1;
                            }
                        }
                    }
                    else if(board[boardSize - 1][0] == computerColour) {
                        if(i == (boardSize - 1)) {
                            int counterLeft = j - 1;
                            while(board[i][counterLeft] == computerColour) {
                                if(counterLeft == (boardSize - 1)) {
                                    score = score + 10;
                                }
                                counterLeft = counterLeft - 1;    
                            }
                        }
                        else if(j == 0) {
                            int counterDown = i + 1;
                            while(board[counterDown][j] == computerColour) {
                                if(counterDown == 0) {
                                    score = score + 10;
                                }
                                counterDown = counterDown + 1;
                            }
                        }
                    }
                    else if(board[boardSize - 1][boardSize - 1] == computerColour) {
                        if(i == (boardSize - 1)) {
                            int counterRight = j + 1;
                            while(board[i][counterRight] == computerColour) {
                                if(counterRight == (boardSize - 1)) {
                                    score = score + 30;
                                }
                                counterRight = counterRight + 1;    
                            }
                        }
                        else if(j == (boardSize - 1)) {
                            int counterDown = i + 1;
                            while(board[counterDown][j] == computerColour) {
                                if(counterDown == (boardSize - 1)) {
                                    score = score + 30;
                                }
                                counterDown = counterDown + 1;
                            }
                        }
                    }            
                }
                
                // Determines if that move is the best possible move so far.
                if(score > highScore) {
                    highScore = score;
                    *row = 'a' + i;
                    *col = 'a' + j;
                }
            }
        }
    }
    printf("Computer places %c at %c%c. \n", computerColour, *row, *col); 
    return;
}

// Determines the winner of the game based on tile count. 
void declareWinner(char board[][26], int boardSize) {
    int countWhite = 0;
    int countBlack = 0;
    
    for(int i = 0; i < boardSize; i++) {
        for(int j = 0; j < boardSize; j++) {
            if(board[i][j] == 'W') {
                countWhite = countWhite + 1;
            }
            else if(board[i][j] == 'B') {
                countBlack = countBlack + 1;
            }
        }
    }
    if(countWhite > countBlack) {
        printf("W player wins.");
    }
    else if(countBlack > countWhite) {
        printf("B player wins.");
    }
    else if(countBlack == countWhite) {
        printf("Draw.");
    }
    return;
}

// Determines the opposite colour of the current colour. 
char oppositeColour(char currentColour) {
    if (currentColour == 'W') {
        return('B');
    }
    else {
        return('W');
    }
}
/*
 * 
 */
int main(void) {
    
    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    
    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_utime;
    double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds 
    
    int boardSize;
    char board[26][26];
    char currentColour;
    char row;
    char column;
    char computerColour;
    int turnCount = 5;
    int inactiveCount = 0;
    int maximumMoves = 0;

    // Asks user for board dimensions.
    printf("Enter the board dimension: ");
    scanf("%d", &boardSize);
    initializeBoard(board, boardSize); 
    maximumMoves = (boardSize * boardSize);
    
    // Asks user if they want to be black or white.
    printf("Computer plays (B/W) : ");
    scanf(" %c", &computerColour);  
    printBoard(board, boardSize);
    
    // Reiterating through turn-based game.
    while((turnCount <= (maximumMoves + 1)) && (inactiveCount < 2)) {
        setPlayerTurn(turnCount, computerColour, &currentColour);
        // Determines if there are any moves to be made. If so, asks for move. If not, goes to next player. If still no moves to be made, ends game.
        if((validMovesLeft(board, boardSize, row, column, currentColour) == true)) {
            recordMove(board, boardSize, currentColour, &row, &column, computerColour);   
            // Checks if move is valid. If move is valid, places piece and flips tiles. If move is not valid, declares opposite colour winner. 
            if(moveChecker(board, boardSize, row, column, currentColour) == true) {
                placePiece(board, row, column, currentColour);
                oppositeColourFlipper(board, boardSize, row, column, currentColour);
                turnCount = turnCount + 1;
                inactiveCount = 0;
                printBoard(board, boardSize);
            }
            else if(moveChecker(board, boardSize, row, column, currentColour) == false) {
                printf("Invalid move. \n");
                printf("%c player wins.", oppositeColour(currentColour));
                break;
            }   
        }
        else if(turnCount > maximumMoves) {
            declareWinner(board, boardSize);
            break;
        }
        else if(validMovesLeft(board, boardSize, row, column, currentColour) == false) {
            printf("%c player has no valid move.\n", currentColour);
            inactiveCount = inactiveCount + 1;
            if(inactiveCount == 1) {
                turnCount = turnCount + 1;
                maximumMoves = maximumMoves + 1;
            }
            else if(inactiveCount == 2) {
                declareWinner(board, boardSize);
                break;
            }        
        }   
    }
        
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;
    double timeEnd = end.tv_sec +
    end.tv_usec / 1000000.0; // in seconds
    double totalTime = timeEnd - timeStart;// totalTime now holds the time (in seconds) it takes to run your code 
    
    return (EXIT_SUCCESS);
}