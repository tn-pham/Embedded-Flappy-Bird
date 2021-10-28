/**
 *	LED matrix driver code adapted from the github link below
 *	https://github.com/MontreaI/BeagleBone-Green-Adafruit-16x32-LED-Matrix-Sample-Code/blob/master/test_ledMatrix.c
 *	
 *	Modified by: Tan Pham
 *  Date: 20 October 2019
 */

/********************************************************************
 *  ORIGINAL HEADER:
 *
 *  File Name: test_ledMatrix.c
 *  Description: A simple program to display pattern on LED Matrix
 *  
 *  About 80% of the code converted from Python to C, source:
 *      https://learn.adafruit.com/connecting-a-16x32-rgb-led-matrix-panel-to-a-raspberry-pi/experimental-python-code
 *-------------------------------------------------------------------
 *  Created by: Janet mardjuki
 *  Date: 3 December 2015
 *  
 *  Modified by: Raymond Chan
 *  Date: 2 August 2018
 ********************************************************************/

#ifndef LED_MATRIX_H
#define LED_MATRIX_H


/**
 *	Start the background thread to control the LED matrix
 *	Must be called before anything else
 */
void ledMatrix_init(void);

/**
 *	Stop the background thread to control the LED matrix
 *	Must be called last to clean up
 */
void ledMatrix_cleanup(void);


/**
 *	Set the screen 
 *  @params:
 *      int newScreen[16][32]: pointer to an array of 16*32 (matrix) - enforced
 */
void ledMatrix_setScreen(int newScreen[16][32]);

#endif