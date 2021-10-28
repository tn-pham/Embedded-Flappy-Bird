// preGameScreen.c

#include "preGameScreen.h"
#include <string.h>
#include <time.h>
#include "ledMatrix.h"
#include "screenText.h"

#define SLEEP_COUNT_DOWN 2
#define ROWS 16
#define COLS 32
#define POS_X 5
#define POS_Y_3_LETTERS 8
#define POS_Y_5_LETTERS 2

#define COUNT_DOWN_COLOR 5

void preEndScreen_init(void)
{
    int screen[ROWS][COLS];
    struct timespec sleepTimer = {SLEEP_COUNT_DOWN, 0};

    // Display 3
    memset(screen, 0, sizeof(screen));
    screenText_printString("three", sizeof("three"), screen, POS_X, POS_Y_5_LETTERS, ROWS, COLS, COUNT_DOWN_COLOR);
    ledMatrix_setScreen(screen);
    nanosleep(&sleepTimer, (struct timespec*) NULL);	

    // Display 2
    memset(screen, 0, sizeof(screen));
    screenText_printString("two", sizeof("two"), screen, POS_X, POS_Y_3_LETTERS, ROWS, COLS, COUNT_DOWN_COLOR);
    ledMatrix_setScreen(screen);
    nanosleep(&sleepTimer, (struct timespec*) NULL);

    // Display 1
    memset(screen, 0, sizeof(screen));
    screenText_printString("one", sizeof("one"), screen, POS_X, POS_Y_3_LETTERS, ROWS, COLS, COUNT_DOWN_COLOR);
    ledMatrix_setScreen(screen);
    nanosleep(&sleepTimer, (struct timespec*) NULL);
}