// screenGameplay.h
// Module to display the game play screen

#ifndef SCREEN_GAMEPLAY_H
#define SCREEN_GAMEPLAY_H

// Cheat system
void gameScreen_setMaxScore(int newMaxScore);

// Display the game play
void gameScreen_init(int speed, int gravity, int newBirdColor);

#endif