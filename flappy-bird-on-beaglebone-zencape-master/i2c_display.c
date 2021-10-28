// i2c_display.c

#include "i2c_display.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>

#define I2CDRV_BUS "/dev/i2c-1"
#define I2C_ADDRESS 0x20
#define LOW_REG 0x14
#define HIGH_REG 0x15
#define LEFT_DIGIT_DIRECTION "/sys/class/gpio/gpio44/direction"
#define LEFT_DIGIT_PATH "/sys/class/gpio/gpio44/value"
#define RIGHT_DIGIT_DIRECTION "/sys/class/gpio/gpio61/direction"
#define RIGHT_DIGIT_PATH "/sys/class/gpio/gpio61/value"

#define DISPLAY_RATE_NANO 5000000
#define DIGIT_COUNT 10

static int score;

static pthread_t displayThread;
static pthread_mutex_t scoreMutex = PTHREAD_MUTEX_INITIALIZER;
static int runFlag = 1;

struct DigitDisplay {
	int digit;
	unsigned char lowReg;
	unsigned char highReg;
};

// Display values
const struct DigitDisplay digitList[] = {
	{0, 0xA1, 0x86},
	{1, 0x01, 0x80},
	{2, 0x31, 0x0E},
	{3, 0x31, 0x8C},
	{4, 0x91, 0x88},
	{5, 0xB0, 0x8C},
	{6, 0xB0, 0x8E},
	{7, 0x21, 0x80},
	{8, 0xB1, 0x8E},
	{9, 0xB1, 0x8C},
};
static int i2cFileDesc;


// Search for index of the list of digit patterns from integer
static int getIndexFromDigit(int digit)
{
	for (int i = 0; i < DIGIT_COUNT; i++) {
		if (digit == (digitList[i]).digit) {
			return i;
		}
	}
	// Should never reach here
	return 0;
}

// Write to a single register
static void writeToReg(unsigned char reg, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = reg;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("I2C: Unable to write i2c register.");
		exit(1);
	}
}

// Draw the digit
static void drawDigit(int digit)
{
	int index = getIndexFromDigit(digit);
	unsigned char low = (digitList[index]).lowReg;
	unsigned char high = (digitList[index]).highReg;
	writeToReg(LOW_REG, low);
	writeToReg(HIGH_REG, high);	
}

// Write
static void writeToFile(char* filePath, char* content)
{
	FILE* file = NULL;

	// Open file
	file = fopen(filePath, "w");
	if (file == NULL) {
		printf("ERROR: Failed to open file %s\n", filePath);
		exit(1);
	}

	// Write
	int writing = fprintf(file, "%s", content);
	if (writing <= 0) {
		printf("Cannot write to file %s\n", filePath);
		exit(1);
	}

	// Close file
	fclose(file);

}

// Draw left digit only
static void I2C_drawLeft(int digit)
{
	writeToFile(LEFT_DIGIT_PATH, "0");
	writeToFile(RIGHT_DIGIT_PATH, "0");
	drawDigit(digit);
	writeToFile(LEFT_DIGIT_PATH, "1");
	writeToFile(RIGHT_DIGIT_PATH, "0");
}

// Draw right digit only
static void I2C_drawRight(int digit)
{
	writeToFile(LEFT_DIGIT_PATH, "0");
	writeToFile(RIGHT_DIGIT_PATH, "0");
	drawDigit(digit);
	writeToFile(LEFT_DIGIT_PATH, "0");
	writeToFile(RIGHT_DIGIT_PATH, "1");
}

// Display thread
void* run14SegDisplayThread(void* args)
{
	// Alternating between 2 digits to display in a fast rate
	while(runFlag) {
		struct timespec sleepTimer = {0, DISPLAY_RATE_NANO};
		
		int leftDigit = score / 10;
		I2C_drawLeft(leftDigit);	
		nanosleep(&sleepTimer, (struct timespec*) NULL);
		
		int rightDigit = score % 10;
		I2C_drawRight(rightDigit);
		nanosleep(&sleepTimer, (struct timespec*) NULL);
	}

	// Clean up
	writeToFile(LEFT_DIGIT_PATH, "0");
	writeToFile(RIGHT_DIGIT_PATH, "0");
	close(i2cFileDesc);

	return NULL;
}

// Init (mostly similar to Dr. Brian Fraser's code since there's only 1 way to do this)
void segDisplay_init(void)
{
	// Set direction
	writeToFile(LEFT_DIGIT_DIRECTION, "out");
	writeToFile(RIGHT_DIGIT_DIRECTION, "out");

	// Config pins
	FILE* config_18 = popen("config-pin P9_18 i2c", "r");
	if (config_18 == NULL) {
		perror("I2C: Unable to config P9_18.\n");
		exit(1);
	}
	pclose(config_18);
	
	FILE* config_17 = popen("config-pin P9_17 i2c", "r");
	if (config_17 == NULL) {
		perror("I2C: Unable to config P9_17.\n");
		exit(1);
	}
	pclose(config_17);
	
	// Set up I2C
	i2cFileDesc = open(I2CDRV_BUS, O_RDWR);
	int result = ioctl(i2cFileDesc, I2C_SLAVE, I2C_ADDRESS);
	if (result < 0) {
		perror("I2C: Unable to set I2C device to slave address.");
		exit(1);
	}

	//set gpio extender to be outputs
   	FILE* setGPIO1 = popen("i2cset -y 1 0x20 0x00 0x00", "r");
	FILE* setGPIO2 = popen("i2cset -y 1 0x20 0x01 0x00", "r");
	if (setGPIO1 == NULL) {
		perror("Could not set GPIO extender.\n");
		exit(1);
	}
	if (setGPIO2 == NULL) {
		perror("Could not set GPIO extender.\n");
		exit(1);
	}

	// Start thread to display
	score = 0;
	pthread_create(&displayThread, NULL, &run14SegDisplayThread, NULL);
}



// Clear both digits
void segDisplay_cleanup(void)
{
	runFlag = 0;
	pthread_join(displayThread,NULL);
}


// Set score to display
void segDisplay_setScore(int newScore)
{
	pthread_mutex_lock(&scoreMutex);
	{
		score = newScore;
	}
	pthread_mutex_unlock(&scoreMutex);
}