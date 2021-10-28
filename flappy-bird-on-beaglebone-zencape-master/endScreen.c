// endScreen.c

#include "screenText.h"
#include <string.h>
#include "ledMatrix.h"

#define COLS 32
#define ROWS 16
#define FIRST_LINE_POS_X 2
#define SECOND_LINE_POS_X 9
#define POS_Y_3_LETTERS 8
#define POS_Y_4_LETTERS 5

#define LOSE_COLOR 5
#define WIN_COLOR 2

// Win screen
void endScreen_win(void)
{
    int screen[ROWS][COLS];
    memset(screen, 0, sizeof(screen));

    screenText_printString("you", sizeof("you"), screen, FIRST_LINE_POS_X, POS_Y_3_LETTERS, ROWS, COLS, WIN_COLOR);
    screenText_printString("win", sizeof("win"), screen, SECOND_LINE_POS_X, POS_Y_3_LETTERS, ROWS, COLS, WIN_COLOR);
    ledMatrix_setScreen(screen);
}

// Lose screen
void endScreen_lose(void)
{
    int screen[ROWS][COLS];
    memset(screen, 0, sizeof(screen));

    screenText_printString("you", sizeof("you"), screen, FIRST_LINE_POS_X, POS_Y_3_LETTERS, ROWS, COLS, LOSE_COLOR);
    screenText_printString("lose", sizeof("lose"), screen, SECOND_LINE_POS_X, POS_Y_4_LETTERS, ROWS, COLS, LOSE_COLOR);
    ledMatrix_setScreen(screen);
}
