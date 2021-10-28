//joystick_control.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include"joystick_control.h"

#define PATH_DOWN "/sys/class/gpio/gpio26"
#define PATH_LEFT "/sys/class/gpio/gpio47"
#define PATH_UP "/sys/class/gpio/gpio46"
#define PATH_RIGHT "/sys/class/gpio/gpio65"

#define BUFFER_SIZE 1024

JOYSTICK_STATE j_state;

// File Management
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

// Read to buffer
static int readFromFile(char* filePath)
{
	FILE* file = NULL;
	
	// Open file
	file = fopen(filePath, "r");
	if (file == NULL) {
		printf("ERROR: Failed to open file %s\n", filePath);
		exit(1);
	}
	
	// Read
	char buffer[BUFFER_SIZE];
	fgets(buffer, BUFFER_SIZE, file);
	
	// Close file
	fclose(file);

	return atoi(buffer);
}


// Init
// Set up a particular pin to read inputs
static void setJoystickPinTakeInput(char* filePath)
{
	char fullPath[BUFFER_SIZE];
	strcpy(fullPath, filePath);
	strcat(fullPath, "/direction");
	writeToFile(fullPath, "in");
}

// Set up to read inputs
void joystick_init(void)
{
	setJoystickPinTakeInput(PATH_UP);
	setJoystickPinTakeInput(PATH_RIGHT);
	setJoystickPinTakeInput(PATH_DOWN);
	setJoystickPinTakeInput(PATH_LEFT);
}


// Read input from a particular pin
static int readPin(char* filePath)
{
	char path[BUFFER_SIZE];
	strcpy(path, filePath);
	strcat(path, "/value");
	return readFromFile(path);
}

// Get joystick state
JOYSTICK_STATE joystick_readJoystick(void)
{	
	// Get joystick state
	if (readPin(PATH_UP) != 1) {
		return J_UP;
	} else if (readPin(PATH_RIGHT) != 1) {
		return J_RIGHT;
	} else if (readPin(PATH_DOWN) != 1) {
		return J_DOWN;
	} else if (readPin(PATH_LEFT) != 1) {
		return J_LEFT;
	} else {
		return J_NONE;
	}
}