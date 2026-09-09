/** @file   logic.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This provides the display and game state logic of the game
*/

#ifndef LOGIC_H
#define LOGIC_H

#include "environment.h"
#include "player.h"
#include <string.h>

//Refreshes and displays the screen
void displayScreen(void);

//Shifts the car row to the desired direction
void moveCarRow(void);

//Probability of adding a new car to the end of the car row
void addCarToRow(void);

//Checks if player hits the car
void playerContact(void);

//Check the IR transmissoin to see if player won or losts
void checkWon(void);

//Sets display for if game is won
void gameWon(void);

//Sets display for if game is lost
void gameLost(void);

//Resets the player to the start and resets the cars (car speed increses)
void nextLevel(void);

//Resets the whole game
void resetGame(void);

//Resets the game for both users using IR 
void resetForBoth(char);

#endif      
