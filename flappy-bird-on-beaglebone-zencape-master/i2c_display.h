// i2c_display.h
// Module to draw numbers on the 14-segment display

#ifndef _I2C_DISPLAY_H_
#define _I2C_DISPLAY_H_

// Initialize I2C settings and start the thread
void segDisplay_init(void);

// Set score to display
void segDisplay_setScore(int newScore);

// Stop displaying digits, turn off
void segDisplay_cleanup(void);


#endif
