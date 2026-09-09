/** @file   game.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This handles all the player movements using the navswitch
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"
#include "logic.h"

//Initializes the player to be in row 7 and column 3
void player_init(void);

//Checks if the navswitch was pushed and call the onpush function
void check_pressed(void);

//Returns the x position of the player
uint8_t getXPos(void);

//Returns the y position of the player
uint8_t getYPos(void);

//Sets the position of the player
void setPos(uint8_t, uint8_t);

#endif