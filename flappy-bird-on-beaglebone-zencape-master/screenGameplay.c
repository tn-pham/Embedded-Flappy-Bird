// screenGameplay.c

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "microphone.h"
#include "ledMatrix.h"
#include "i2c_display.h"
#include "endScreen.h"
//#include "buzzer.h"

#define ROWS 16
#define COLS 32

#define SLEEP_NANO_SEC 5000000		// 5 milisec

#define COUNTER_RESET_VALUE -1		// Value = -1 for val++ to make it 0 right away
#define OBSTACLE_IN_FRAMES 8		// Obstacle every 8 units
#define JUMP_COUNTER_MAX 40			// 20 frames until next jump

#define SPEED_INCREASE_RATE 10		// How much speed increased
#define SPEED_INCREASE_FREQUENCY 10	// Increase speed every 10 scores
#define SPEED_MAX_IN_FRAMES 1		// Screen shifted left every 1 frame

#define SPEED_BIAS 110				// bias - speed = real speed
#define MAX_SPEED 100				// Max speed
#define MIN_SPEED 20				// Min speed
#define GRAVITY_BIAS 110			// bias - gravity = real gravity
#define MAX_GRAVITY 100				// Max gravity
#define MIN_GRAVITY 20				// Min gravity
#define MAX_MAX_SCORE 99			// Max max score
#define MIN_MAX_SCORE 1				// Min max score

#define DEFAULT_BIRD_HEIGHT 6		// Default bird position: [6][1]

#define MAX_COLOR 7					// Number of colors in game
#define MAX_PIPE_HEIGHT 9			// Max height of the upper pipe (for randomization)
#define PIPE_GAP_SIZE 5				// How big the gap between pipes

static pthread_mutex_t maxScoreMutex = PTHREAD_MUTEX_INITIALIZER;

static int screen[ROWS][COLS];

// Frequency to update
static int speedInFrames;
static int gravityInFrames;

static int maxScore;
static int score;
static int pipeColor;
static int birdColor;
static int birdHeight;

static _Bool isGameRunning = false;

static void setSpeed(int newSpeed)
{
	// Range 20-100
	assert(newSpeed >= MIN_SPEED);
	assert(newSpeed <= MAX_SPEED);
	
	// Calculate speed in frames (invert)
	// +1 to ensure speedInFrames never reaches 0
	speedInFrames = SPEED_BIAS - newSpeed;
}

static void setGravity(int newGravity)
{
	// Range 1-100
	assert(newGravity >= MIN_GRAVITY);
	assert(newGravity <= MAX_GRAVITY);
	
	// Calculate speed in frames (invert)
	// +1 to ensure gravityInFrames never reaches 0
	gravityInFrames = GRAVITY_BIAS - newGravity;
}

// Cheat system to change default max score
void gameScreen_setMaxScore(int newMaxScore)
{
	// Range 1-99
	assert(newMaxScore >= MIN_MAX_SCORE);
	assert(newMaxScore <= MAX_MAX_SCORE);
	
	// Only update if game is running
	if (!isGameRunning) {
		return;
	}

	// Only set to higher than score
	if (maxScore <= score) {
		return;
	}
	
	// Lock in case there are 2 processes calling to change maxScore
	pthread_mutex_lock(&maxScoreMutex);
	{
		maxScore = newMaxScore;
	}
	pthread_mutex_unlock(&maxScoreMutex);
}

static _Bool isCollided(void)
{
	if (screen[birdHeight][1] != 0) {
		return true;
	}
	return false;
}

// Change color of all pipes
static void changePipeColor(void)
{
	// Pipe color from 1 to MAX_COLOR and must be different from bird color
	int newColor = (pipeColor + 1) % MAX_COLOR + 1;
	if (newColor == birdColor) {
		newColor++;
	}
	
	// Change color of all pipes
	for(int row = 0; row < ROWS; row++) {
        for(int col = 1; col < COLS; col++) {
			if (screen[row][col] == pipeColor) {
				screen[row][col] = newColor;
			}
		}
	}
	pipeColor = newColor;
}

// Create pipes in a specific column
static void setPipes(int colNum)
{
    // Gaps between 1-6 to 9-14
	int upperPipeHeight = (rand() % MAX_PIPE_HEIGHT) + 1;
	int lowerPipeHeight = upperPipeHeight + PIPE_GAP_SIZE;
	
	// Set pipes
	for (int row = 0; row < ROWS; row++) {
		if (row < upperPipeHeight || row > lowerPipeHeight) {
			screen[row][colNum] = pipeColor;
		}	
	}
}

// Shift the whole screen left by 1
static void shiftLeft(_Bool flag)
{
    for(int row = 0; row < ROWS; row++) {
        for(int col = 1; col < COLS; col++) {
            screen[row][col-1] = screen[row][col];
            if(col == (COLS - 1)) {
                screen[row][col] = 0;
            }
        }
    }
    
    if(flag) {
        setPipes(COLS - 1);
    }
}

static void gameLose(void)
{
	//buzzer_buzz();
	endScreen_lose();
	isGameRunning = false;
}

static void gameWin(void)
{
	// Display winning score on 14-seg display
	// Limit to 99 since we can't do more than 2 digits
	if (score > 99) {
		score = 99;
	}
	segDisplay_setScore(score);

	endScreen_win();
	isGameRunning = false;
}

static void runGame(void)
{
	// Set up counters to update
	int speedCounter = 0;
	int gravityCounter = 0;
	int obstacleCounter = 0;
	int jumpCounter = 0;
	_Bool isJumping = false;
	
	// Game is running
	isGameRunning = true;

	// Every frame
	while(1) {
		/***********************************************************************************
			Game updates
		************************************************************************************/

		// Remove bird dot (will be added back later)
		screen[birdHeight][1] = 0;				
				
		// Check speed update frequency to shift the game screen left by 1 unit
		if (speedCounter == speedInFrames) {
			speedCounter = COUNTER_RESET_VALUE;

			// Shift left by 1 and generate next obstacle if needed
			if (obstacleCounter == OBSTACLE_IN_FRAMES) {
				obstacleCounter = COUNTER_RESET_VALUE;
				shiftLeft(true);
			} else {
				shiftLeft(false);
			}
			
			obstacleCounter++;

			// Hitting any pipe means game over
			if (isCollided()) {
				gameLose();
				break;
			}

			/***********************************************************************************
			*	Checking the game state after a shift
			************************************************************************************/

			// An obstacle is passed, update score
			if (screen[0][0] != 0) {
				score++;
				
				// Win if reach max score
				if (score > maxScore) {	
					gameWin();
					break;
				}
				
				// Increase difficulty (speed) every SPEED_INCREASE_FREQUENCY scores
				if (score % SPEED_INCREASE_FREQUENCY == 0) {
					speedInFrames -= SPEED_INCREASE_RATE;
					// Limit the speed to max speed, only change color if speed increased
					if (speedInFrames < SPEED_MAX_IN_FRAMES) {
						speedInFrames = SPEED_MAX_IN_FRAMES;
					} else {				
						// Change pipe colors
						changePipeColor();
					}
				}
			}
		}
		
		// Check if there is an input for a jump
		// Must finish first jump before doing another one
		int jumpValue;
		if (!isJumping) {
			jumpValue = microphone_getValue();
			// Start jumping from next frame
			if (jumpValue > 0) {
				isJumping = true;
				jumpCounter = 0;
			}
		}
		// While doing the jump
		else {
			// Jump duration depends on input value (jumpValue)
			if (jumpValue > 0) {
				// Inverted height
				birdHeight--;
				if (birdHeight < 0 ) {
					birdHeight = 0;
					jumpValue = 0;
				}

				jumpValue--;
			}
			// Cool down between 2 jumps
			if (jumpCounter > JUMP_COUNTER_MAX) {
				isJumping = false;
			}
			jumpCounter++;
		}
		
		// Check gravity update frequency to lower bird's position
		if (gravityCounter == gravityInFrames) {
			gravityCounter = COUNTER_RESET_VALUE;
			
			// Inverted height
			birdHeight++;
			if (birdHeight >= (COLS - 1)) {
				// Bird drops on the ground, game over
				gameLose();
				break;
			}
		}
			
		// Update counters
		speedCounter++;
		gravityCounter++;
		
		
		/***********************************************************************************
			Update UI
		************************************************************************************/
		
		// Put back bird on screen
		screen[birdHeight][1] = birdColor;
		
		// Update LED matrix
		ledMatrix_setScreen(screen);
		
		// Update score on 14-seg display
		segDisplay_setScore(score);

		// Sleep until next frame
		struct timespec sleepTimer = {0, SLEEP_NANO_SEC};
		nanosleep(&sleepTimer, (struct timespec*) NULL);
	}
}

void gameScreen_init(int speed, int gravity, int newBirdColor)
{
	srand(time(0));
	// Buzzer
	//buzzer_init();

	// Set up starting screen
	memset(screen, 0, sizeof(screen));

	// Bird
	birdHeight = DEFAULT_BIRD_HEIGHT;
	birdColor = newBirdColor;
	screen[birdHeight][1] = birdColor;
	
	// Pipes (generate a new pipe color)
	pipeColor = birdColor;
	changePipeColor();

	// Initial pipes
	setPipes(15);
    setPipes(23);
    setPipes(31);
	
	// Score
	score = 0;
	maxScore = MAX_MAX_SCORE;
	
	// Speed
	setSpeed(speed);
		
	// Gravity
	setGravity(gravity);
	
	// Draw starting screen
	ledMatrix_setScreen(screen);
	
	// Run the game loop
	runGame();
}