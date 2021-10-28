// microphone.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "microphone.h"

#define A2D_FILE_VOLTAGE "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"

#define BUFFER_SIZE 400
#define THRESHOLD 150

#define RANGE_FOR_NORMALIZATION 30
#define MAX_NORMALIZED_VALUE 5

#define SLEEP_NANO_COOL_DOWN 200000000
#define SLEEP_NANO_SEC 1000000		// 1 milisec

static pthread_t microphone_thread;
static int runFlag = 1;

static int samples[BUFFER_SIZE];
static int processedSoundValue = 0;

// Convert raw values into needed range
static int normalizeSound(int rawValue, int currentAverage)
{
	// Calculate the difference and put in the appropriate range
	int result = (rawValue - currentAverage - THRESHOLD) / RANGE_FOR_NORMALIZATION;
	
	// Limit the value
	if (result > MAX_NORMALIZED_VALUE) {
		return MAX_NORMALIZED_VALUE;
	}
	return result;
}

// Get raw values
static int getVoltageReading(void)
{
	// Open file
	FILE* file = fopen(A2D_FILE_VOLTAGE, "r");
	if(!file){
		printf("Error: Unable to open the voltage input file.\n");
		exit(-1);
	}
	
	// Get the raw voltage reading
	int a2dReading = 0;
	int itemsRead = fscanf(file,"%d",&a2dReading);
	if(itemsRead <= 0){
		printf("Error: Unable to read from voltage input file\n");
		exit(-1);
	}
	fclose(file);
	
	return a2dReading;
}

// Thread function
void* micThread(void* args)
{
	int currentIndex = 0;
	int currentSum = 0;
	int currentAverage = 0;
	
	// Initialize samples
	for (int i = 0; i < BUFFER_SIZE; i++) {
		samples[i] = getVoltageReading();
		currentSum += samples[i];
	}
	currentAverage = currentSum / BUFFER_SIZE;

	while(runFlag) {
		int rawValue = getVoltageReading();
		
		// If we consider it as a needed sound, normalize it
		if (rawValue - currentAverage >= THRESHOLD) {
			processedSoundValue = normalizeSound(rawValue, currentAverage);
			
			// No need to take more input for 0.2s to reduce noise
			struct timespec sleepTimer = {0, SLEEP_NANO_COOL_DOWN};
			nanosleep(&sleepTimer, (struct timespec*) NULL);
		} else {
			// Treat as background noise
			processedSoundValue = 0;
			
			// Cache sum in order not to loop through the whole array all the time
			currentSum = currentSum - samples[currentIndex] + rawValue;
			currentAverage = currentSum / BUFFER_SIZE;
			
			// Replace least recent value
			samples[currentIndex] = rawValue;
			
			// Recalculate index for the circular array
			currentIndex = (currentIndex + 1) % BUFFER_SIZE;
		}
		//printf("val: %d, sum: %d, avg: %d\n", rawValue, currentSum, currentAverage);
		// Sleep until next update
		struct timespec sleepTimer = {0, SLEEP_NANO_SEC};
		nanosleep(&sleepTimer, (struct timespec*) NULL);
	}
	
	return NULL;
}

// Get the converted value
int microphone_getValue(void)
{
	return processedSoundValue;
}

void microphone_init(void)
{
	// Set up
	memset(samples, 0, sizeof(samples));
	
	// Create thread
	pthread_create(&microphone_thread, NULL, &micThread, NULL);
}


void microphone_cleanup(void)
{
	runFlag = 0;
	pthread_join(microphone_thread,NULL);
}