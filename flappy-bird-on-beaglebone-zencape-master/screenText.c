// screenText.c

#include "screenText.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LETTER_SIZE 5

// Encoded letters
static void drawA(int screen[ROWS][COLS], int shiftX, int shiftY, int color)
{
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][1+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;
	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][4+shiftY] = color;
}

static void drawB(int screen[ROWS][COLS], int shiftX, int shiftY, int color)
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][2+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
}

static void drawC(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{

	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
}

static void drawD(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][1+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;

}

static void drawE(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
}

static void drawF(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;

}
static void drawG(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
	
}

static void drawH(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
	
}
static void drawI(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][2+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
	screen[4+shiftX][4+shiftY] = color;
}

static void drawJ(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][2+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;

}
static void drawK(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][2+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}

static void drawL(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}
static void drawM(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;
	screen[1+shiftX][4+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][4+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][4+shiftY] = color;

}
static void drawN(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][4+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][4+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;
	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][4+shiftY] = color;

}
static void drawO(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][4+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][4+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
}
static void drawP(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;

}

static void drawQ(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}
static void drawR(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
	
}

static void drawS(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{

	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;
	screen[2+shiftX][4+shiftY] = color;

	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;
}

static void drawT(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][2+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][2+shiftY] = color;

}
static void drawU(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}
static void drawV(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][4+shiftY] = color;

	screen[2+shiftX][1+shiftY] = color;
	screen[2+shiftX][3+shiftY] = color;

	screen[3+shiftX][1+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][2+shiftY] = color;
	
}
static void drawW(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][0+shiftY] = color;
	screen[1+shiftX][4+shiftY] = color;

	screen[2+shiftX][0+shiftY] = color;
	screen[2+shiftX][2+shiftY] = color;
	screen[2+shiftX][4+shiftY] = color;

	screen[3+shiftX][0+shiftY] = color;
	screen[3+shiftX][2+shiftY] = color;
	screen[3+shiftX][4+shiftY] = color;

	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}
static void drawX(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][1+shiftY] = color;
	screen[3+shiftX][3+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][4+shiftY] = color;
	
}
static void drawY(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][4+shiftY] = color;

	screen[1+shiftX][1+shiftY] = color;
	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][2+shiftY] = color;

	screen[4+shiftX][2+shiftY] = color;

}
static void drawZ(int screen[ROWS][COLS], int shiftX, int shiftY, int color) 
{
	screen[0+shiftX][0+shiftY] = color;
	screen[0+shiftX][1+shiftY] = color;
	screen[0+shiftX][2+shiftY] = color;
	screen[0+shiftX][3+shiftY] = color;

	screen[1+shiftX][3+shiftY] = color;

	screen[2+shiftX][2+shiftY] = color;

	screen[3+shiftX][1+shiftY] = color;

	screen[4+shiftX][0+shiftY] = color;
	screen[4+shiftX][1+shiftY] = color;
	screen[4+shiftX][2+shiftY] = color;
	screen[4+shiftX][3+shiftY] = color;

}

// Table of functions to draw letters
static void (*letter_drawers[])(int[ROWS][COLS], int, int, int) = {
	drawA, drawB, drawC, drawD, drawE,
	drawF, drawG, drawH, drawI, drawJ,
	drawK, drawL, drawM, drawN, drawO,
	drawP, drawQ, drawR, drawS, drawT,
	drawU, drawV, drawW, drawX, drawY,
	drawZ
};

// Print any string to screen
// Omit if there's not enough space to print the next char
void screenText_printString(char* text, int textSize, int screen[ROWS][COLS], int posX, int posY, int sizeX, int sizeY, int color)
{
	// Not counting the termination char
	textSize--;

	// Validations
	if (textSize < 1) {
		return;
	}
	
	if (posX < 0) {
		return;
	}

	if (posY < 0) {
		return;
	}
	
	if (posY + LETTER_SIZE > sizeY) {
		return;
	}

	// Print
	int currentPosY = posY;
	for (int i = 0; i < textSize; i++) {
		if (currentPosY + LETTER_SIZE > sizeY) {
			return;
		} else {
			int index = text[i] - 'a';

			// If letter
			if (index >= 0 || index <= ('z' - 'a')) {
				letter_drawers[index](screen, posX, currentPosY, color);
			}
			// If not space, skip
			else if (text[i] != ' ') {
				continue;
			}

			// If space or letter
			currentPosY += LETTER_SIZE + 1;
		}
	}
}