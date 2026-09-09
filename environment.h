/** @file   game.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This initializes all the required elements and stores all the global variables
*/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "pio.h"
#include "system.h"
#include "navswitch.h"
#include "pacer.h"
#include <stdlib.h>
#include <time.h>
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"

//Global Variables
#define PACER_RATE 500
#define DISPLAY_RATE 500
#define NAVSWITCH_RATE 500
#define MESSAGE_RATE 15
#define CAR_ROW_NUMBER 3
#define INITIAL_MOVE_RATE 1
#define INITIAL_FLASH_RATE 0
#define PLAYER_BLINK_RATE 5


#define LED_PIO PIO_DEFINE(PORT_C, 2)
#define BUTTON_PIO PIO_DEFINE(PORT_D, 7)

extern pio_t rowList[];
extern pio_t colList[];

extern float carsMoveRate; //rate at which the cars move
extern float ledFlashRate; //rate at which the LED flashes (depends on relative distance)

extern bool gameOver;
extern bool hasStarted;

extern bool ledState; 

extern uint16_t noWins; //number of wins
extern uint16_t noLosses; //number of losses

extern uint64_t stepCount; //number of steps the player has taken

extern bool playerBlink; //bool that toggles if player should be displayed (for player blink)

typedef struct {
    bool isRight;
    uint8_t rowNum;
    uint8_t array[5];
} carArray;

extern carArray carArrays[CAR_ROW_NUMBER]; //array that contains all the car rows

//Initializes all the environment variable and drivers
void environment_init (void);

#endif