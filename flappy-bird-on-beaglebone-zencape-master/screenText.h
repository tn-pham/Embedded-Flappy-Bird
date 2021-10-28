// screenText.h
// Module to print any string on the given 2D array
// by putting values in the right elements of the 2D array

#ifndef SCREEN_TEXT_H_
#define SCREEN_TEXT_H_

#define ROWS 16
#define COLS 32

void screenText_printString(char* text, int textSize, int screen[ROWS][COLS], int posX, int posY, int sizeX, int sizeY, int color);

#endif