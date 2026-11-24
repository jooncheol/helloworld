/**
 * ENGG1110 Problem Solving by Programming
 *
 * Course Project
 *
 * I declare that the project here submitted is original
 * except for source material explicitly acknowledged,
 * and that the same or closely related material has not been
 * previously submitted for another course.
 * I also acknowledge that I am aware of University policy and
 * regulations on honesty in academic work, and of the disciplinary
 * guidelines and procedures applicable to breaches of such
 * policy and regulations, as contained in the website.
 *
 * University Guideline on Academic Honesty:
 *   https://www.cuhk.edu.hk/policy/academichonesty/
 *
 * Student Name  : <your name>
 * Student ID    : <your student ID>
 * Class/Section : <your class/section>
 * Date          : <date>
 */


// [DO NOT MODIFY]
// DO NOT ADD new header files

#include <stdio.h>

#define MAX_ROWS 20
#define MAX_COLS 40
#define INITIAL_RANGE 1

// In Part 2, BOMB_FUSE must be changed.
#define BOMB_FUSE 4

/* Square states specified in the project document. */
#define CORRIDOR 7
#define WALL 9
#define BRICK_EMPTY 8
#define BRICK_BOOST 6
#define BOOST 5
#define PLAYER 1
#define PLAYER_ON_BOMB 3
#define BOMB 2
#define EXPLODE 4
#define EXPLODE_BOOST -1
// END OF [DO NOT MODIFY]

/* Indexes in the state[] array specified in the project document. */
#define STATE_FIELDS 15     // no. of fields
#define PLAYER_ROW 0		// player location
#define PLAYER_COL 1		// player location
#define PLAYER_ALIVE 2 		// player status (for game end checks)
#define EXPLOSION_RANGE 3 	// current bomb explosion range
#define BOMB_ACTIVE 4 		// current bomb state
#define BOMB_ROW 5			// current bomb row index
#define BOMB_COL 6			// current bomb column index
#define BOMB_TIMER 7		// current bomb timer value
#define ACTION_COUNT 8		// number of user valid actions (WASD + PB)
#define MOVE_COUNT 9		// number of user valid movement (WASD)
#define BOMB_COUNT 10		// total bombs dropped
#define BRICKS_REMAINS 11	// game statistics
#define BRICKS_DESTROYED 12	// game statistics
#define BOOSTS_DROPPED 13	// game statistics
#define BOOSTS_COLLECTED 14	// game statistics

// The GAME STATUS constants
#define GAME_RUNNING 0
#define GAME_WON 1
#define GAME_LOST 2

// [DO NOT MODIFY] 
void initialise(int board[][MAX_COLS], int size[], int state[]);
void displayBoard(int board[][MAX_COLS], int size[], int state[]);
void displayLegend(int state[]);
int movePlayer(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange);
int dropBomb(int board[][MAX_COLS], int state[]);
void advanceTime(int board[][MAX_COLS], int size[], int state[]);
void explodeBomb(int board[][MAX_COLS], int size[], int state[]);
void endGameMessage(int state[], int win); // e.g., win can take GAME_WON or GAME_LOST or otherwise
// END OF [DO NOT MODIFY]

/* Define your helper functions prototype below. You can implement it somewhere else in this file.*/



/* DO NOT DECLARE EXTRA GLOBAL VARIABLES */

int main(void) {
    // Suggested starter variables
    int board[MAX_ROWS][MAX_COLS];
    int size[2];
    int state[STATE_FIELDS];
    
    char command;

    // Initialize the game board
    initialise(board, size, state);

    while (1) {
        displayBoard(board, size, state);
        displayLegend(state);
        printf("Command [W/A/S/D = move, B = bomb, P = pause, Q = quit]: ");
        fflush(stdout);

        // The space before %c skips Enter and other whitespace.
        if (scanf(" %c", &command) != 1) {
            break;
        }
        if (command >= 'a' && command <= 'z') {
            command = command - 'a' + 'A';
        }
        if (command == 'Q') {
            break;
        }
        if (command != 'W' && command != 'A' && command != 'S' &&
            command != 'D' && command != 'B' && command != 'P') {
            printf("Error: enter W, A, S, D, B, P, or Q.\n");
            continue;
        }
    }

    return 0;
}

void initialise(int board[][MAX_COLS], int size[], int state[]) {
    // The hardcoded game board
    int start[9][15] = {
        {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},
        {9,1,7,7,8,7,7,7,7,6,7,7,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,7,7,7,8,7,7,7,7,7,8,7,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,6,7,7,7,7,8,7,7,7,7,8,7,7,9},
        {9,7,9,7,9,7,9,7,9,7,9,7,9,7,9},
        {9,7,7,7,7,8,7,7,7,7,7,7,7,7,9},
        {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9}
    };
    int row = 9;
    int col = 15;
    int r, c, i;

    // Start the counters at zero, then set the initial player values.
    size[0] = row;
    size[1] = col;
    for (i = 0; i < STATE_FIELDS; i++) {
        state[i] = 0;
    }
    state[PLAYER_ALIVE] = 1;
    state[EXPLOSION_RANGE] = INITIAL_RANGE;

    // Copy the map and find the player and all breakable bricks.
    for (r = 0; r < row; r++) {
        for (c = 0; c < col; c++) {
            board[r][c] = start[r][c];
            if (board[r][c] == PLAYER) {
                state[PLAYER_ROW] = r;
                state[PLAYER_COL] = c;
            }
            if (board[r][c] == BRICK_EMPTY || board[r][c] == BRICK_BOOST) {
                state[BRICKS_REMAINS]++;
            }
        }
    }
}

void displayBoard(int board[][MAX_COLS], int size[], int state[]) {
    int r, c;

    // Give every tile two characters so the columns stay aligned.
    for (r = 0; r < size[0]; r++) {
        for (c = 0; c < size[1]; c++) {
            switch (board[r][c]) {
                case WALL: printf("[]"); break;
                case BRICK_EMPTY:
                case BRICK_BOOST: printf("()"); break;
                case PLAYER: printf("@@"); break;
                case PLAYER_ON_BOMB: printf("@B"); break;
                case BOMB: printf("BB"); break;
                case BOOST: printf("EX"); break;
                case EXPLODE:
                case EXPLODE_BOOST: printf("##"); break;
                default: printf("  "); break;
            }
        }
        printf("\n");
    }

    // Show bomb details only while a bomb is active.
    printf("Actions=%d  Moves=%d  Bombs=%d  Range=%d  Soft bricks=%d\n",
           state[ACTION_COUNT], state[MOVE_COUNT], state[BOMB_COUNT],
           state[EXPLOSION_RANGE], state[BRICKS_REMAINS]);
    if (state[BOMB_ACTIVE]) {
        printf("Bomb 1: timer=%d at (%d,%d)\n", state[BOMB_TIMER],
               state[BOMB_ROW], state[BOMB_COL]);
    }
    printf("Destroyed=%d  Boosts dropped=%d  Boosts collected=%d\n",
           state[BRICKS_DESTROYED], state[BOOSTS_DROPPED], state[BOOSTS_COLLECTED]);
}

void displayLegend(int state[]) {
    // Print the symbols separately from the command prompt.
    (void)state;
    printf("Legend: [] wall, () soft brick, @@ player, @B player on bomb, BB bomb, EX boost, ## explosion\n");
}



int movePlayer(int board[][MAX_COLS], int size[], int state[], int rowChange, int colChange) {
    // Process a "move" command from user.
    // return value indicates success or fail
    
}

int dropBomb(int board[][MAX_COLS], int state[]) {
    // Process a dropBomb command from user.
    // return value indicates success or fail
    
}



void advanceTime(int board[][MAX_COLS], int size[], int state[]) {
    // Advance game time; explode bomb if needed
}

void explodeBomb(int board[][MAX_COLS], int size[], int state[]) {
    // Explode the bomb; read the data from state[]/board[][]
}



void endGameMessage(int state[], int win) {
    // Display End Game Message and Statistics
}
