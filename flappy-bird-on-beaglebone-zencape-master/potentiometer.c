// potentiometer.c

#include "potentiometer.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define MAX_ANALOG_VALUE 4095
#define COLOR_COUNT 7
#define RANGE (MAX_ANALOG_VALUE / COLOR_COUNT)
#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

// Get the raw value from potentiometer
static int getRawValue(void)
{
	// Open file
	FILE* file = fopen(A2D_FILE_VOLTAGE0, "r");
	if (!file) {
		printf("ERROR: Unable to open voltage input file.\n");
		exit(-1);
	}
	
	// Get reading
	int value = 0;
	int readResult = fscanf(file, "%d", &value);
	if (readResult <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}
	
	// Close file
	fclose(file);
	return value;
}

// Get the converted value (size of array)
int pot_getValue(void)
{
	int digitalValue = getRawValue() / RANGE + 1;
	if (digitalValue >= COLOR_COUNT) {
		digitalValue = COLOR_COUNT;
	}
	return digitalValue;
}