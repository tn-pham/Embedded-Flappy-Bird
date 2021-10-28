/**
 *	LED matrix driver code adapted from the github link below
 *	https://github.com/MontreaI/BeagleBone-Green-Adafruit-16x32-LED-Matrix-Sample-Code/blob/master/test_ledMatrix.c
 *	
 *	Modified by: Tan Pham
 *  Date: 20 October 2019
 */

/********************************************************************
 *  ORIGINAL HEADER:
 *
 *  File Name: test_ledMatrix.c
 *  Description: A simple program to display pattern on LED Matrix
 *  
 *  About 80% of the code converted from Python to C, source:
 *      https://learn.adafruit.com/connecting-a-16x32-rgb-led-matrix-panel-to-a-raspberry-pi/experimental-python-code
 *-------------------------------------------------------------------
 *  Created by: Janet mardjuki
 *  Date: 3 December 2015
 *  
 *  Modified by: Raymond Chan
 *  Date: 2 August 2018
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "ledMatrix.h"

/*** GLOBAL VARIABLE ***/
/* GPIO PATH */
#define GPIO_PATH "/sys/class/gpio/"

/* GPIO Pins Definition */
#define RED1_PIN 8     // UPPER
#define GREEN1_PIN 80
#define BLUE1_PIN 78
#define RED2_PIN 76 // LOWER
#define GREEN2_PIN 79
#define BLUE2_PIN 74
#define CLK_PIN 73      // Arrival of each data
#define LATCH_PIN 75    // End of a row of data
#define OE_PIN 71       // Transition from one row to another
#define A_PIN 72        // Row select
#define B_PIN 77
#define C_PIN 70

#define S_IWRITE "S_IWUSR"

#define SLEEP_EXPORT_NANO 330000000
#define SLEEP_REFRESH 50000

#define ROWS 16
#define COLS 32

// 2 screens (1 main and 1 buffer - alternating)
static int screen1[ROWS][COLS];
static int screen2[ROWS][COLS];
// Buffer flag to signal which screen to display
static int displayScreen;

/* FILES HANDLER */
static int fileDesc_red1;
static int fileDesc_blue1;
static int fileDesc_green1;
static int fileDesc_red2;
static int fileDesc_blue2;
static int fileDesc_green2;
static int fileDesc_clk;
static int fileDesc_latch;
static int fileDesc_oe;
static int fileDesc_a;
static int fileDesc_b;
static int fileDesc_c;

// Thread
static pthread_t matrixThreadId;
static pthread_mutex_t matrixMutex = PTHREAD_MUTEX_INITIALIZER;
static int runFlag = 1;

/**
 *	Nanosleep
*/
static void sleepNanoSec(int nanoseconds)
{
	long seconds = 0;
	struct timespec sleepTimer = {seconds, nanoseconds};
	nanosleep(&sleepTimer, (struct timespec*) NULL);
}

/**
 * exportAndOut
 * Export a pin and set the direction to output
 * @params
 *  int pinNum: the pin number to be exported and set for output
 */
static void exportAndOut(int pinNum)
{
    // Export the gpio pins
    FILE *gpioExP = fopen(GPIO_PATH "export", "w");
    if ( gpioExP == NULL ){
        printf("ERROR: Unable to open export file.\n");
        exit(-1);
    }
    fprintf(gpioExP, "%d", pinNum);
    fclose(gpioExP);
        
    sleepNanoSec(SLEEP_EXPORT_NANO);
    
    // Change the direction into out
    char fileNameBuffer[1024];
    sprintf(fileNameBuffer, GPIO_PATH "gpio%d/direction", pinNum);
        
    FILE *gpioDirP = fopen(fileNameBuffer, "w");
    fprintf(gpioDirP, "out");
    fclose(gpioDirP);

    return;
}

/**
 * ledMatrix_setupPins
 * Setup the pins used by the led matrix, by exporting and set the direction to out
 */
static void ledMatrix_setupPins(void)
{   
    // !Upper led
    exportAndOut(RED1_PIN);
    fileDesc_red1 = open("/sys/class/gpio/gpio8/value", O_WRONLY, S_IWRITE);
    exportAndOut(GREEN1_PIN);
    fileDesc_green1 = open("/sys/class/gpio/gpio80/value", O_WRONLY, S_IWRITE);
    exportAndOut(BLUE1_PIN);
    fileDesc_blue1 = open("/sys/class/gpio/gpio78/value", O_WRONLY, S_IWRITE);

    // Lower led
    exportAndOut(RED2_PIN);
    fileDesc_red2 = open("/sys/class/gpio/gpio76/value", O_WRONLY, S_IWRITE);
    exportAndOut(GREEN2_PIN);
    fileDesc_green2 = open("/sys/class/gpio/gpio79/value", O_WRONLY, S_IWRITE);
    exportAndOut(BLUE2_PIN);
    fileDesc_blue2 = open("/sys/class/gpio/gpio74/value", O_WRONLY, S_IWRITE);

    // Timing
    exportAndOut(CLK_PIN);
    fileDesc_clk = open("/sys/class/gpio/gpio73/value", O_WRONLY, S_IWRITE);
    exportAndOut(LATCH_PIN);
    fileDesc_latch = open("/sys/class/gpio/gpio75/value", O_WRONLY, S_IWRITE);
    exportAndOut(OE_PIN);
    fileDesc_oe = open("/sys/class/gpio/gpio71/value", O_WRONLY, S_IWRITE);

    // Row Select
    exportAndOut(A_PIN);
    fileDesc_a = open("/sys/class/gpio/gpio72/value", O_WRONLY, S_IWRITE);
    exportAndOut(B_PIN);
    fileDesc_b = open("/sys/class/gpio/gpio77/value", O_WRONLY, S_IWRITE);
    exportAndOut(C_PIN);
    fileDesc_c = open("/sys/class/gpio/gpio70/value", O_WRONLY, S_IWRITE); 

    return;
}

/** 
 *  ledMatrix_clock
 *  Generate the clock pins
 */
static void ledMatrix_clock(void)
{
    // Bit-bang the clock gpio
    // Notes: Before program writes, must make sure it's on the 0 index
    lseek(fileDesc_clk, 0, SEEK_SET);
    write(fileDesc_clk, "1", 1);
    lseek(fileDesc_clk, 0, SEEK_SET);
    write(fileDesc_clk, "0", 1);

    return;
}

/**
 *  ledMatrix_latch
 *  Generate the latch pins
 */
static void ledMatrix_latch(void)
{
    lseek(fileDesc_latch, 0, SEEK_SET);
    write(fileDesc_latch, "1", 1);
    lseek(fileDesc_latch, 0, SEEK_SET);
    write(fileDesc_latch, "0", 1);

    return;
}

/**
 *  ledMatrix_bitsFromInt
 *  Convert integer passed on into bits and put in array
 *  @params:
 *      int * arr: pointer to array to be filled with bits
 *      int input: integer to be converted to bits
 */
static void ledMatrix_bitsFromInt(int * arr, int input)
{
    arr[0] = input & 1;

    arr[1] = input & 2;
    arr[1] = arr[1] >> 1;

    arr[2] = input & 4;
    arr[2] = arr[2] >> 2;

    return;
}

/**
 *  ledMatrix_setRow
 *  Set LED Matrix row
 *  @params:
 *      int rowNum: the rowNumber to be inputted to row pins
 */
static void ledMatrix_setRow(int rowNum)
{
    // Convert rowNum single bits from int
    int arr[3] = {0, 0, 0};
    ledMatrix_bitsFromInt(arr, rowNum);

    // Write on the row pins
    char a_val[2];
    sprintf(a_val, "%d", arr[0]);
    lseek(fileDesc_a, 0, SEEK_SET);
    write(fileDesc_a, a_val, 1);

    char b_val[2];
    sprintf(b_val, "%d", arr[1]);
    lseek(fileDesc_b, 0, SEEK_SET);
    write(fileDesc_b, b_val, 1);

    char c_val[2];
    sprintf(c_val, "%d", arr[2]);
    lseek(fileDesc_c, 0, SEEK_SET);
    write(fileDesc_c, c_val, 1);


    return;
}

/**
 *  ledMatrix_setColourTop
 *  Set the colour of the top part of the LED
 *  @params:
 *      int colour: colour to be set
 */
static void ledMatrix_setColourTop(int colour)
{
    int arr[3] = {0, 0, 0};
    ledMatrix_bitsFromInt(arr, colour);

    // Write on the colour pins
    char red1_val[2];
    sprintf(red1_val, "%d", arr[0]);
    lseek(fileDesc_red1, 0, SEEK_SET);
    write(fileDesc_red1, red1_val, 1);

    char green1_val[2];
    sprintf(green1_val, "%d", arr[1]);
    lseek(fileDesc_green1, 0, SEEK_SET);
    write(fileDesc_green1, green1_val, 1);

    char blue1_val[2];
    sprintf(blue1_val, "%d", arr[2]);
    lseek(fileDesc_blue1, 0, SEEK_SET);
    write(fileDesc_blue1, blue1_val, 1);    

    return;
}

/**
 *  ledMatrix_setColourBottom
 *  Set the colour of the bottom part of the LED
 *  @params:
 *      int colour: colour to be set
 */
static void ledMatrix_setColourBottom(int colour)
{
    int arr[3] = {0,0,0};
    ledMatrix_bitsFromInt(arr, colour);

    // Write on the colour pins
    char red2_val[2];
    sprintf(red2_val, "%d", arr[0]);
    lseek(fileDesc_red2, 0, SEEK_SET);
    write(fileDesc_red2, red2_val, 1);

    char green2_val[2];
    sprintf(green2_val, "%d", arr[1]);
    lseek(fileDesc_green2, 0, SEEK_SET);
    write(fileDesc_green2, green2_val, 1);

    char blue2_val[2];
    sprintf(blue2_val, "%d", arr[2]);
    lseek(fileDesc_blue2, 0, SEEK_SET);
    write(fileDesc_blue2, blue2_val, 1);      

    return;
}

/**
 *  ledMatrix_refresh
 *  Fill the LED Matrix with the respective pixel colour
 */
static void ledMatrix_refresh()
{
	// Pointer to screen to display
	int (*screen)[ROWS][COLS];
	
	// Check flag to see which one to display
	if (displayScreen == 1) {
		screen = &screen1;
	} else {
		screen = &screen2;
	}

	// Display the screen
	for ( int rowNum = 0; rowNum < 8; rowNum++ ) {
		lseek(fileDesc_oe, 0, SEEK_SET);
		write(fileDesc_oe, "1", 1); 
		ledMatrix_setRow(rowNum);
		for ( int colNum = 0; colNum < 32;  colNum++) {
			ledMatrix_setColourTop((*screen)[rowNum][colNum]);
			ledMatrix_setColourBottom((*screen)[rowNum + 8][colNum]);
			ledMatrix_clock();
		}
		ledMatrix_latch();
		lseek(fileDesc_oe, 0, SEEK_SET);
		write(fileDesc_oe, "0", 1); 
		sleepNanoSec(SLEEP_REFRESH);
	}
	
	return;
}

/**
 *	Copy array
 */
static void ledMatrix_copyArray(int destination[ROWS][COLS], int source[ROWS][COLS])
{
    for(int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            destination[i][j] = source[i][j];
        }
    }
}


// Threading stuff
/**
 *	Background thread to control the LED matrix
 */
void* ledThread(void* arg)
{
	// Keep refreshing until told to stop the thread
	while (runFlag) {
		ledMatrix_refresh();
	}
	
	return NULL;
}

/**
 *	Start the background thread to control the LED matrix
 *	Must be called before anything else
 */
void ledMatrix_init(void)
{
	// Set up pins
	ledMatrix_setupPins();
	
	// Set screen to display
	displayScreen = 1;
	memset(screen1, 0, sizeof(screen1));
	memset(screen2, 0, sizeof(screen2));

    screen1[2][3] = 1;
	
	// Create thread
	pthread_create(&matrixThreadId, NULL, ledThread, NULL);
}

/**
 *	Stop the background thread to control the LED matrix
 *	Must be called last to clean up
 */
void ledMatrix_cleanup(void)
{
	runFlag = 0;
	pthread_join(matrixThreadId, NULL);
    
    memset(screen1, 0, sizeof(screen1));
    memset(screen2, 0, sizeof(screen2));
    ledMatrix_refresh();
}

/**
 *	Set the screen 
 *  @params:
 *      int newScreen[ROWS][COLS]: pointer to an array of 16*32 (matrix) - enforced
 */
void ledMatrix_setScreen(int newScreen[ROWS][COLS])
{
	pthread_mutex_lock(&matrixMutex);
    {
		// Copy to the right buffer then set screen to the new buffer
        if (displayScreen == 1) {
            ledMatrix_copyArray(screen2, newScreen);
            displayScreen = 2;
        } else {
            ledMatrix_copyArray(screen1, newScreen);
            displayScreen = 1;
        }
    }
    pthread_mutex_unlock(&matrixMutex);
}
