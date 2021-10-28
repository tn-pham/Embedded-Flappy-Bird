// welcomeScreen.c

#include "ledMatrix.h"
#include "joystick_control.h"
#include <stdio.h>
#include <time.h>
//#include "button.h"

#define Y 3 // Yellow 
#define B 4 // Blue
#define W 7 // White
#define R 1 // Red
#define N 0 //No color
#define SLEEP_NANO_SEC 20000000

void welcomeScreen_init(void)
{
    // Flappy bird icon
    int screenBird[16][32] = {
        {N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, N, N, B, B, B, B, B, B, N, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, B, B, Y, Y, Y, B, W, W, B, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, B, Y, Y, Y, Y, B, W, W, W, W, B, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, B, B, B, B, Y, Y, Y, B, W, W, W, B, W, B, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, B, W, W, W, W, B, Y, Y, B, W, W, W, B, W, B, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, B, W, W, W, W, W, B, Y, Y, B, W, W, W, W, B, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, B, Y, W, W, W, Y, B, Y, Y, Y, B, B, B, B, B, B, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, B, Y, Y, Y, B, Y, Y, Y, B, R, R, R, R, R, R, B, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, B, B, B, Y, Y, Y, B, R, B, B, B, B, B, B, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, B, Y, Y, Y, Y, Y, Y, B, R, R, R, R, R, B, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, B, B, Y, Y, Y, Y, Y, B, B, B, B, B, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, N, B, B, B, B, B, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N},
        {N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N}
    };

    ledMatrix_setScreen(screenBird);

    while(1) {
        /*if (button_isButtonCLicked()) {
           return;
        }*/

        if (joystick_readJoystick() != J_NONE) {
            return;
        }

        // Avoid busy wait as much as possible
		struct timespec sleepTimer = {0, SLEEP_NANO_SEC};
		nanosleep(&sleepTimer, (struct timespec*) NULL);
    }
}