/** @file   game.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This handles all the player movements using the navswitch
*/

#include "player.h"

uint8_t xpos;
uint8_t ypos;

//Initializes the player to be in row 7 and column 3
void player_init(void)
{
    navswitch_init ();
    xpos = 2;
    ypos = 6;
}

//Updates position of the player on navswitch push
static void onpush(uint8_t direction)
{   
    switch (direction) {
        case NAVSWITCH_NORTH: //NORTH
            if (ypos == 1) {
                if (stepCount == 253) {
                    ir_uart_putc((char) 253);
                    gameWon ();
                    return;
                }
                stepCount += 1;
                nextLevel ();
                break;
            }
            if (ypos > 0) {
                ypos -= 1;
                stepCount += 1;
            }
            break;
        case NAVSWITCH_SOUTH: //SOUTH
            if (ypos < 6) {
                ypos += 1;
                stepCount -=1;
            }
            break;
        case NAVSWITCH_EAST: //EAST
            if (xpos < 4) {
                xpos += 1;
            }
            break;
        case NAVSWITCH_WEST: //WEST
            if (xpos > 0) {
                xpos -= 1;
            }
            break;
    }
}

//Checks if the navswitch was pushed and call the onpush function
void check_pressed(void)
{
    navswitch_update ();
    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        onpush(NAVSWITCH_EAST);
        return;
    }

    if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        onpush(NAVSWITCH_WEST);
        return;
    }

    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        onpush(NAVSWITCH_NORTH);
        return;
    }

    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        onpush(NAVSWITCH_SOUTH);
        return;
    }
}

//Returns the x position of the player
uint8_t getXPos(void)
{
    return xpos;
}

//Returns the y position of the player
uint8_t getYPos(void)
{
    return ypos;
}

//Sets the position of the player
void setPos(uint8_t newXPos, uint8_t newYPos)
{
    xpos = newXPos;
    ypos = newYPos;
}
