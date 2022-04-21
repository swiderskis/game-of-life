#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void printBoard(int boardWidth, int boardHeight, int board[]);
void boardUpdate(int boardWidth, int boardHeight, int board[], int *boardPointer[]);

int menuMain();
int menuPreset();
int menuQuit();
int optionCheck(int option, int optionMin, int optionMax, char menuText[]);

void delay(unsigned int mseconds);

int main() {
    srand(time(NULL)); // Random seed

    int boardWidth = 5, boardHeight = 5;
    static int boardSizeMin = 5, boardSizeMax = 25;
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
            boardWidth = optionCheck(boardWidth, boardSizeMin, boardSizeMax, boardWidthText);
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
            FILE *preset = NULL;

            option = menuPreset();

            // Loads preset chosen in menuPreset and modifies board size
            switch(option) {
            case 11:
                boardWidth = 6;
                boardHeight = 6;
                preset = fopen("stillLifes/block.txt", "r");
                break;
            case 12:
                boardWidth = 6;
                boardHeight = 5;
                preset = fopen("stillLifes/beehive.txt", "r");
                break;
            case 13:
                boardWidth = 6;
                boardHeight = 6;
                preset = fopen("stillLifes/loaf.txt", "r");
                break;
            case 14:
                boardWidth = 5;
                boardHeight = 5;
                preset = fopen("stillLifes/boat.txt", "r");
                break;
            case 15:
                boardWidth = 5;
                boardHeight = 5;
                preset = fopen("stillLifes/tub.txt", "r");
                break;
            case 21:
                boardWidth = 5;
                boardHeight = 5;
                preset = fopen("oscillators/blinker.txt", "r");
                break;
            case 22:
                boardWidth = 6;
                boardHeight = 6;
                preset = fopen("oscillators/toad.txt", "r");
                break;
            case 23:
                boardWidth = 6;
                boardHeight = 6;
                preset = fopen("oscillators/beacon.txt", "r");
                break;
            case 24:
                boardWidth = 19;
                boardHeight = 19;
                preset = fopen("oscillators/pulsar.txt", "r");
                break;
            case 25:
                boardWidth = 20;
                boardHeight = 13;
                preset = fopen("oscillators/pentadecathlon.txt", "r");
                break;
            case 31:
                boardWidth = 25;
                boardHeight = 25;
                preset = fopen("spaceships/glider.txt", "r");
                break;
            case 32:
                boardWidth = 25;
                boardHeight = 9;
                preset = fopen("spaceships/lwss.txt", "r");
                break;
            case 33:
                boardWidth = 25;
                boardHeight = 11;
                preset = fopen("spaceships/mwss.txt", "r");
                break;
            case 34:
                boardWidth = 25;
                boardHeight = 11;
                preset = fopen("spaceships/hwss.txt", "r");
                break;
            }

            board = realloc(board, boardWidth*boardHeight*sizeof(int));
            boardPointer = realloc(boardPointer, boardWidth*boardHeight*sizeof(int*));

            for (int i = 0; i < boardHeight; i++) {
                for (int j = 0; j < boardWidth; j ++) {
                    fscanf(preset, "%d", &board[i*boardWidth + j]);
                }
            }

            for (int i = 0; i < boardHeight; i++) {
                for (int j = 0; j < boardWidth; j++) {
                    boardPointer[i*boardWidth + j] = &board[i*boardWidth + j];
                }
            }

            free(preset);

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

    option = optionCheck(option, optionMin, optionMax, menuText);

    return option;
}

// Preset menu
// Presets taken from https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
int menuPreset() {
    int option = 0, optionMin = 1, optionMax = 3;
    char menuTextPreset[] = "Please choose a preset category:\n"
                            "1) Still lifes\n"
                            "2) Oscillators\n"
                            "3) Spaceships\n";

    char menuTextStillLifes[] = "Please choose which still life to display:\n"
                                "1) Block\n"
                                "2) Beehive\n"
                                "3) Loaf\n"
                                "4) Boat\n"
                                "5) Tub\n";

    char menuTextOscillators[] = "Please choose which oscillator to display:\n"
                                 "1) Blinker\n"
                                 "2) Toad\n"
                                 "3) Beacon\n"
                                 "4) Pulsar\n"
                                 "5) Pentadecathlon\n";

    char menuTextSpaceships[] = "Please choose which spaceship to display:\n"
                                "1) Glider\n"
                                "2) LWSS\n"
                                "3) MWSS\n"
                                "4) HWSS\n";

    option = optionCheck(option, optionMin, optionMax, menuTextPreset);

    switch(option) {
    case 1:
        optionMax = 5;
        option = 10 + optionCheck(option, optionMin, optionMax, menuTextStillLifes);
        break;
    case 2:
        optionMax = 5;
        option = 20 + optionCheck(option, optionMin, optionMax, menuTextOscillators);
        break;
    case 3:
        optionMax = 4;
        option = 30 + optionCheck(option, optionMin, optionMax, menuTextSpaceships);
        break;
    }

    return option;
}

// Quit menu
int menuQuit() {
    int option = 0, optionMin = 1, optionMax = 2;
    char menuText[] = "Would you like to return to the main menu?\n"
                      "1) Yes\n"
                      "2) No\n";

    option = optionCheck(option, optionMin, optionMax, menuText);

    return option;
}

// Checks if the option chosen in menus is valid
int optionCheck (int option, int optionMin, int optionMax, char menuText[]) {
    system("cls");
    printf("%s", menuText);
    scanf("%d", &option);

    while (option > optionMax || option < optionMin) {
        system("cls");
        printf("ERROR: Invalid selection\n\n");
        printf("%s", menuText);
        scanf("%d", &option);
    }

    return option;
}

// Delay function to wait between loops
// Taken from https://stackoverflow.com/questions/27447195/how-to-use-delay-function-in-c-using-codeblocks-13-12mingw
void delay(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
