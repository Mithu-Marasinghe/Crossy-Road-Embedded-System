/** @file   game.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This provides the main game module that handles the pacer and the timing of all events
*/

#include "player.h"
#include "environment.h"
#include "logic.h"


int main (void)
{
    environment_init ();
    player_init ();

    uint16_t navswitch_tick = 0;
    uint16_t display_tick = 0;
    uint16_t move_left_tick = 0;
    uint16_t flash_tick = 0;
    uint16_t player_blink_tick = 0;

    //Whole game functions below using a parcer
    while (1)
    {
        pacer_wait();
        //Checks if on start screen
        if (!hasStarted) {
            if (pio_input_get(BUTTON_PIO)) {
                hasStarted = true;
                resetForBoth ((char) 255);
            } else {
                if (ir_uart_read_ready_p()){
                    char recievedChar = ir_uart_getc ();
                    
                    if (recievedChar == (char) 255) {
                        hasStarted = true;
                        resetGame ();
                    }
                }
                tinygl_update ();
            }
            continue;
        }

        //Checks if game is over and user presses reset button
        if (pio_input_get(BUTTON_PIO) && gameOver) {
            resetForBoth ((char) 255);
        }

        //Checks for new IR reading
        if (ir_uart_read_ready_p()){
            checkWon();
        }

        //Process tiny_gl text if game is over
        if (gameOver) {
            tinygl_update();
            continue; 
        }

        navswitch_tick++;
        display_tick++;
        move_left_tick++;
        flash_tick++;
        player_blink_tick++;

        if (player_blink_tick >= PACER_RATE / PLAYER_BLINK_RATE && !gameOver) {
            playerBlink = !playerBlink;
            player_blink_tick = 0;
        }

        //Flashes the LED depending on the distance between the 2 users
        if (flash_tick >= PACER_RATE / ledFlashRate && !gameOver) {
            if (!ledState) {
                pio_output_high(LED_PIO);
            } else {
                pio_output_low(LED_PIO);
            }

            ledState = !ledState;
            flash_tick = 0;
        }

        //Checks for navswitch input
        if (navswitch_tick >= PACER_RATE / NAVSWITCH_RATE && !gameOver) {
            check_pressed ();
            navswitch_tick = 0;
        }

        //Refreshes display and checks for player contact
        if (display_tick >= PACER_RATE / DISPLAY_RATE && !gameOver) {
            displayScreen ();
            playerContact ();
            display_tick = 0;
        }

        //Moves the car array to the left and may add a new car to the end of it
        if (move_left_tick >= PACER_RATE / carsMoveRate && !gameOver) {
            moveCarRow();
            addCarToRow();

            ir_uart_putc((char) stepCount);
            move_left_tick = 0;
        }
    }
}