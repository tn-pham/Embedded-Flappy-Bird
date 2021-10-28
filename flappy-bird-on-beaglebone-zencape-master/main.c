#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "screenGameplay.h"
#include "microphone.h"
#include "ledMatrix.h"
#include "i2c_display.h"
#include "joystick_control.h"
#include "welcomeScreen.h"
#include "screenText.h"
#include "udp_listener.h"
#include "preGameScreen.h"
#include "setupScreen.h"
//#include "button.h"

#define SLEEP_END_SCREEN 3
int runFlag = 1;

int main()
{
    // Set ups
    ledMatrix_init();
    microphone_init();
    segDisplay_init();
    joystick_init();
    //button_init();
    UDP_init(&runFlag);
    
    // Game loop
    while (runFlag) {
        welcomeScreen_init();
        
        int color;
        int speed;
        int gravity;
        
        setupScreen_init(&color, &speed, &gravity);
        preEndScreen_init();
        gameScreen_init(speed, gravity, color);

        struct timespec sleepTimer = {SLEEP_END_SCREEN, 0};
		nanosleep(&sleepTimer, (struct timespec*) NULL);
    }
      
    // Clean up
    segDisplay_cleanup();
    microphone_cleanup();
    ledMatrix_cleanup();

}