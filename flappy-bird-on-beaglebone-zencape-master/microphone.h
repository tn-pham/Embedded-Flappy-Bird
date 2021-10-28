// microphone.h
// Module to read microphone values, filter noise
// and return processed values in specified range

#ifndef MICROPHONE_H
#define MICROPHONE_H

void microphone_init(void);
void microphone_cleanup(void);

int microphone_getValue(void);

#endif