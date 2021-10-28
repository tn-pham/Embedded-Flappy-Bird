// joystick_control.h
// Module to read joystick input

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Joystick states
typedef enum { 
    J_NONE,
    J_UP,
    J_DOWN,
    J_LEFT,
    J_RIGHT
} JOYSTICK_STATE;

// Set up to read inputs
void joystick_init(void);

// Get joystick state
JOYSTICK_STATE joystick_readJoystick(void);

#endif