// setupScreen.c

#include "setupScreen.h"
#include <string.h>
#include <time.h>
#include "ledMatrix.h"
#include "screenText.h"
#include "joystick_control.h"
#include "potentiometer.h"
#include <stdio.h>
//#include "button.h"

#define BAR_START_Y 10
#define SLEEP_NANO_REFRESH 5000000
#define BIRD_COUNT 5
#define SLEEP_NANO_DEBOUNCE 200000000

#define ROWS 16
#define COLS 32
#define FIRST_LINE_POS_X 2
#define SECOND_LINE_POS_X 9
#define TEXT_POS_Y 2
#define BAR_LENGTH_DIVIDER 5
#define TEXT_COLOR 5

static int screen[ROWS][COLS];
static int currentChoiceIndex = 1;
static int currentColor = 5;

typedef struct {
    int speed;
    int gravity;
} bird;

// Pre-sets of bird stats
static bird birds[] = {
    {40, 40},
    {50, 50},
    {60, 60},
    {70, 70},
    {80, 80},
};

static void drawBars(void)
{
    int birdSpeed = birds[currentChoiceIndex].speed / BAR_LENGTH_DIVIDER;
    int birdGravity = birds[currentChoiceIndex].gravity / BAR_LENGTH_DIVIDER;

    // Draw bar for Strength at row 3-5
    for (int i = BAR_START_Y; i < (BAR_START_Y + birdSpeed); i++) {
        screen[3][i] = currentColor;
        screen[4][i] = currentColor;
        screen[5][i] = currentColor;
    }

    // Draw bar for Gravity at row 10-12
    for (int i = BAR_START_Y; i < BAR_START_Y + birdGravity; i++) {
        screen[10][i] = currentColor;
        screen[11][i] = currentColor;
        screen[12][i] = currentColor;
    }
}

static void sleepDebounce(void)
{
    // Debounce timer
    struct timespec sleepTimer = {0, SLEEP_NANO_DEBOUNCE};
    nanosleep(&sleepTimer, (struct timespec*) NULL);
}

static int getInputJoystick(void)
{
    int result = 0;

    // Get input from joystick
    JOYSTICK_STATE j_state = joystick_readJoystick();
    if (j_state == J_UP) {
        currentChoiceIndex++;
        if (currentChoiceIndex >= BIRD_COUNT) {
            currentChoiceIndex = 0;
        }
    } else if (j_state == J_DOWN) {
        currentChoiceIndex--;
        if (currentChoiceIndex < 0) {
            currentChoiceIndex = BIRD_COUNT - 1;
        }
    } else if (j_state == J_LEFT || j_state == J_RIGHT) {
        result = 1;
    }
    /*else if (button_isButtonCLicked()) {
        return 1;
    }*/
    
    sleepDebounce();

    return result;
}

static void setScreen(void)
{
    memset(screen, 0, sizeof(screen));
    screenText_printString("s", sizeof("s"), screen, FIRST_LINE_POS_X, TEXT_POS_Y, ROWS, COLS, TEXT_COLOR);
    screenText_printString("w", sizeof("w"), screen, SECOND_LINE_POS_X, TEXT_POS_Y, ROWS, COLS, TEXT_COLOR);
    
    // Get color from pot
    currentColor = pot_getValue();
    drawBars();

    ledMatrix_setScreen(screen);
}

void setupScreen_init(int* color, int* speed, int* gravity)
{
    setScreen();
    sleepDebounce();
    
    while(1) {
        setScreen();

        // Refresh rate
        struct timespec sleepTimer = {0, SLEEP_NANO_REFRESH};
        nanosleep(&sleepTimer, (struct timespec*) NULL);

        if (getInputJoystick()) {
            *color = currentColor;
            *speed = birds[currentChoiceIndex].speed;
            *gravity = birds[currentChoiceIndex].gravity;
            break;
        }
    }	
}