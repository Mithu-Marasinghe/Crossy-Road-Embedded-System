/** @file   game.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This initializes all the required elements and stores all the global variables
*/

#include "environment.h"

pio_t rowList[] = {
        LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
        LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
    };

pio_t colList[] = {
        LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO, 
        LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
    };

float carsMoveRate = INITIAL_MOVE_RATE;
float ledFlashRate = INITIAL_FLASH_RATE;

bool gameOver = false;
uint64_t stepCount = 0;
bool hasStarted = false;

bool ledState = false;

uint16_t noWins = 0;
uint16_t noLosses = 0;

bool playerBlink = false;


carArray carArrays[3];

//Initializes all the environment variable and drivers
void environment_init (void)
{
    system_init ();
    navswitch_init ();

    pacer_init(PACER_RATE);
    ir_uart_init();

    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("Press to Start");

    pio_config_set(LED_PIO, PIO_OUTPUT_LOW);
    pio_config_set(BUTTON_PIO, PIO_INPUT);

    for (size_t i = 0; i < 7; i++)
        pio_config_set(rowList[i], PIO_OUTPUT_HIGH);

    for (size_t j = 0; j < 5; j++)
        pio_config_set(colList[j], PIO_OUTPUT_HIGH);

    //fill car rows to be empty
    carArrays[0] = (carArray){false, 1, {0,0,0,0,0}};
    carArrays[1] = (carArray){true, 3, {0,0,0,0,0}};
    carArrays[2] = (carArray){false, 5, {0,0,0,0,0}};
}