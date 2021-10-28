#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include "button.h"

#define MAX_LENGTH 1024
#define SLEEP_EXPORT_NANO 330000000

#define GPIO_NUM "44"
#define GPIO_44 "/sys/class/gpio/gpio" GPIO_NUM
#define GPIO_PATH "/sys/class/gpio/"

//P8 2 = ground,
//P8 12 = gpio 44 = button output
//P9 2 = voltage input into button
// resistor = 1K ohm (red resistor)

static void writeToFile(char *fileName, char *value)
{
	FILE *file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Error opening file (%s)\n", fileName);
		exit(-1);
	}
	fprintf(file, "%s", value);
	fclose(file);
}


static char readFromFile(char *fileName){
	FILE* file = fopen(fileName, "r");

	if (file == NULL) {
		printf("Error opening file (%s)\n", fileName);
		exit(-1);
	}
	char buff[MAX_LENGTH];
	fgets(buff, MAX_LENGTH, file);

	// Close
	fclose(file);
	return buff[0];
}


void button_init(void)
{
	// Export the gpio pin
    FILE *gpioExP = fopen(GPIO_PATH "export", "w");
    if ( gpioExP == NULL ){
        printf("ERROR: Unable to open export file.\n");
        exit(-1);
    }
    fprintf(gpioExP, "%s", GPIO_NUM);
    fclose(gpioExP);
        
    struct timespec sleepTimer = {0, SLEEP_EXPORT_NANO};
	nanosleep(&sleepTimer, (struct timespec*) NULL);
	
	// Set direction
	DIR* buttDir = opendir(GPIO_44);
	if(ENOENT == errno){
		writeToFile(GPIO_44 "/direction", "in");
	}
	closedir(buttDir);
}


int button_isButtonCLicked(void)
{
	char ch = readFromFile(GPIO_44 "/value");
	if(ch == '1')
		return 1;
	
	return 0;
}
