#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void printBoard(int boardWidth, int boardHeight, int board[]);
void boardUpdate(int boardWidth, int boardHeight, int board[], int *boardPointer[]);
void delay(unsigned int mseconds);
int menuMain();
int menuPreset();
int menuQuit();
int optionCheck(int option, int optionMin, int optionMax, char menuText[]);

int main() {
    srand(time(NULL)); // Random seed

    int boardWidth = 5, boardHeight = 5;
    int boardSizeMin = 5, boardSizeMax = 25;
    int generation = 0;
    int option = 0;

    char boardWidthText[] = "Please input the desired width of the board (min 5, max 25):\n";
    char boardHeightText[] = "Please input the desired height of the board (min 5, max 25):\n";

    int *board = NULL;
    board = malloc(boardWidth*boardHeight*sizeof(int));

    int **boardPointer = NULL;
    boardHeight = malloc(boardWidth*boardHeight*sizeof(int*));

    while (1) {
        option = 0;

        option = menuMain();

        if (option == 1) {
            system("cls");
            printf("%s", boardWidthText);
            scanf("%d", &boardWidth);
            boardWidth = optionCheck(boardWidth, boardSizeMin, boardSizeMax, boardWidthText);

            system("cls");
            printf("%s", boardHeightText);
            scanf("%d", &boardHeight);
            boardHeight = optionCheck(boardHeight, boardSizeMin, boardSizeMax, boardHeightText);

            board = realloc(board, boardWidth*boardHeight*sizeof(int));
            boardPointer = realloc(boardPointer, boardWidth*boardHeight*sizeof(int*));

            for (int i = 0; i < boardHeight; i++) {
                for (int j = 0; j < boardWidth; j++) {
                    boardPointer[i*boardWidth + j] = &board[i*boardWidth + j];
                }
            }

            // Initialise board with random values (0 = dead cell, 1 = alive cell)
            for (int i = 0; i < boardHeight; i++) {
                for (int j = 0; j < boardWidth; j++) {
                    board[i*boardWidth + j] = rand() % 2;
                }
            }
        } else if (option == 2) {
            option = 3;
        }

        if (option != 3) {
            generation = 0;

            // Board evolution
            while(1) {
                generation++;
                system("cls");
                printBoard(boardWidth, boardHeight, board);
                printf("\nGeneration %d\n",generation);
                printf("Press ESC to return to menu\n");
                boardUpdate(boardWidth, boardHeight, board, boardPointer);
                delay(400);

                if (kbhit() == 1) {
                    break;
                }
            }

            option = menuQuit();

        } else {
            break; // Closes program if menuMain sets option to 3
        }

        if (option == 2) { // Closes program if menuQuit sets option to 2
            break;
        }
    }

    free(board);
    free(boardPointer);

    return 0;
}

// Prints current board state
void printBoard(int boardWidth, int boardHeight, int board[]) {
    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {

            switch (board[i*boardWidth + j]) {
            case 0:
                printf("  ");
                break;
            case 1:
                printf("%c ", 254);
                break;
            }
        }
        printf("\n");
    }
}

// Updates board state
void boardUpdate(int boardWidth, int boardHeight, int board[], int *boardPointer[]) {
    int sum = 0;
    int boardCopy[boardWidth*boardHeight];

    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            boardCopy[i*boardWidth + j] = board[i*boardWidth + j];
        }
    }

    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            sum = 0;

            // Counts number of neighbours for each cell
            // Taken from https://www.geeksforgeeks.org/program-for-conways-game-of-life/
            for (int l = -1; l < 2; l++) {
                for (int m = -1; m < 2; m++) {
                    if ((l + i >= 0 && l + i < boardHeight) && (m + j >= 0 && m + j < boardWidth)) {
                        sum += boardCopy[((l + i)*boardWidth) + m + j];
                    }
                }
            }

            sum -= boardCopy[i*boardWidth + j]; // Removes count of itself as a neighbour from above

            if (sum < 2 || sum > 3) {
                *boardPointer[i*boardWidth + j] = 0;
            } else if (sum == 2) {
                *boardPointer[i*boardWidth + j] = boardCopy[i*boardWidth + j];
            } else if (sum == 3) {
                *boardPointer[i*boardWidth + j] = 1;
            }
        }
    }
}

// Main menu
int menuMain() {
    int option = 0, optionMin = 1, optionMax = 3;
    char menuText[] = "Please choose an option:\n"
                      "1) Random board\n"
                      "2) Preset board\n"
                      "3) Quit\n";

    system("cls");
    printf("%s", menuText);
    scanf("%d", &option);

    option = optionCheck(option, optionMin, optionMax, menuText);

    return option;
}

int menuPreset() {
    //int option = 0, optionMin = 1, optionMax = 2;
    //char menuText[] =

    //return option;
}

// Quit menu
int menuQuit() {
    int option = 0, optionMin = 1, optionMax = 2;
    char menuText[] = "Would you like to return to the main menu?\n"
                      "1) Yes\n"
                      "2) No\n";

    system("cls");
    printf("%s", menuText);
    scanf("%d", &option);

    option = optionCheck(option, optionMin, optionMax, menuText);

    return option;
}

// Checks if the option chosen in menus is valid
int optionCheck (int option, int optionMin, int optionMax, char menuText[]) {
    while (option > optionMax || option < optionMin) {
        system("cls");
        printf("ERROR: ");
        printf("%s", menuText);
    }

    return option;
}

// Delay function to wait between loops
// Taken from https://stackoverflow.com/questions/27447195/how-to-use-delay-function-in-c-using-codeblocks-13-12mingw
void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
