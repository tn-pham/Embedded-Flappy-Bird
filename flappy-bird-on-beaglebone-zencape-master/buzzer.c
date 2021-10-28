#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
//#include "buzzer.h"

#define DURATION 500000000

void writeToFile(char *fileName, char *value)
{
	FILE *file = fopen(fileName, "w");
	if (file == NULL) {
		printf("Error opening file (%s)\n", fileName);
		exit(-1);
	}
	fprintf(file, "%s", value);
	fclose(file);
}


void buzzer_init(void)
{
	DIR* buzzDir = opendir("/sys/class/pwm/pwmchip0/pwm0/");
	if(ENOENT == errno){
		writeToFile("/sys/class/pwm/pwmchip0/export","0");
	}

	struct timespec delay = {0, 330000000};
	nanosleep(&delay,(struct timespec *) NULL);

	closedir(buzzDir);
}

void buzzer_buzz(void)
{
	system("cd");
	system(" echo 1000000 | sudo tee /sys/class/pwm/pwmchip0/pwm0/period");
	system(" echo 500000 | sudo tee /sys/class/pwm/pwmchip0/pwm0/duty_cycle");
	system(" echo 1 | sudo tee /sys/class/pwm/pwmchip0/pwm0/enable");

	struct timespec delay = {0, DURATION};
	nanosleep(&delay,(struct timespec *) NULL);

	system(" echo 0 | sudo tee /sys/class/pwm/pwmchip0/pwm0/enable");
}
