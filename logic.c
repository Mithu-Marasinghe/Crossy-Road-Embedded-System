/** @file   logic.c
    @author Mithu Marasinghe, Flynn O'Sullivan
            mma538, fos19
    @date   16/10/25
    @note   This provides the display and game state logic of the game
*/

#include "logic.h"
#include <stdio.h>

static uint8_t current_column = 0;
static uint8_t prev_col = 20;

//Displays all the columns at a high frequency
static void display_column (uint8_t current_column)
{
    uint8_t ypos = getYPos ();
    uint8_t xpos = getXPos ();

    if (prev_col != 20) {
        pio_output_high(colList[prev_col]);
    }
    
    for (size_t r = 0; r < 7; r++) {
        pio_output_high(rowList[r]);
    }

    for (size_t current_row = 0; current_row < 7; current_row++) {
        if (xpos == current_column && ypos == current_row && playerBlink) {
            pio_output_low (rowList[current_row]);
        } else {
            bool found = false;

            for (size_t i = 0; i < CAR_ROW_NUMBER; i++) {
                if ((carArrays[i].rowNum == current_row) && (carArrays[i].array[current_column] == 1) && (!found)) {
                    pio_output_low (rowList[current_row]);
                    found = true;
                } else if (!found) {
                    pio_output_high (rowList[current_row]);
                }
            }
        }
    }
    pio_output_low(colList[current_column]);
    
    prev_col = current_column;
}

//Refreshes and displays the screen
void displayScreen(void)
{
    display_column (current_column);
        
    current_column++;

    if (current_column > (LEDMAT_COLS_NUM - 1))
    {
        current_column = 0;
    }      
}

//Shifts the car row to the desired direction
void moveCarRow(void)
{
    for (size_t s = 0; s < CAR_ROW_NUMBER; s++) {
        if (carArrays[s].isRight) {
            for (size_t i = LEDMAT_COLS_NUM - 1; i > 0; i--) {
                carArrays[s].array[i] = carArrays[s].array[i - 1];
            }
            carArrays[s].array[0] = 0;
        } else {
            for (size_t i = 0; i < LEDMAT_COLS_NUM - 1; i++) {
                carArrays[s].array[i] = carArrays[s].array[i + 1];
            }
            carArrays[s].array[4] = 0;
        }
    }
}

//Probability of adding a new car to the end of the car row
void addCarToRow(void)
{
    for (size_t i = 0; i < CAR_ROW_NUMBER; i++)
    {
        srand(TCNT0 ^ TCNT1);
        int num = (rand() % 4);
        if (num == 1)
        {
            if (carArrays[i].isRight)
            {
                carArrays[i].array[0] = 1;
            }
            else
            {
                carArrays[i].array[4] = 1;
            }
        }
    }
}


//Checks if player hits the car
void playerContact(void)
{
    uint8_t ypos = getYPos ();
    uint8_t xpos = getXPos ();

    for (size_t i = 0; i < CAR_ROW_NUMBER; i++) {
        if (ypos == carArrays[i].rowNum && carArrays[i].array[xpos] == 1) {
            gameLost ();
            break;        
        }
    }
}

//check the IR transmissoin to see if player won or losts
void checkWon(void)
{
    uint8_t recievedChar = ir_uart_getc ();
    if (recievedChar == 255) {
        ledFlashRate = 0;
        pio_output_low(LED_PIO);
        ledState = false;
        resetGame ();
        return;
    }
    if (recievedChar == 254) {
        gameWon ();
        return;
    } 
    if (recievedChar == 253) {
        gameLost ();
        return;
    }
    if (recievedChar > stepCount) {
        ledFlashRate = recievedChar - stepCount;
        if ((recievedChar - stepCount) > 10) {
            gameLost ();
        }
    } else {
        ledFlashRate = 0;
        pio_output_low(LED_PIO);
        ledState = false;
    }
}

//Sets display for if game is won
void gameWon(void)
{
    noWins += 1;
    resetGame ();

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "W:%d L:%d", noWins, noLosses);

    tinygl_text(buffer);
    tinygl_update();

    gameOver = true;
}

//Sets display for if game is lost
void gameLost(void)
{
    noLosses += 1;
    resetGame ();

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "W:%d L:%d", noWins, noLosses);

    tinygl_text(buffer);

    tinygl_update();
    ir_uart_putc((char) 254);
    gameOver = true;
}

//Resets the positions of all the cars in each row
static void resetCars(void)
{
    for (size_t i = 0; i < CAR_ROW_NUMBER; i++) {
        uint8_t carArrayNew[5] = {0};

        for (size_t j = 0; j < LEDMAT_COLS_NUM - 1; j++) {
            int num = (rand() % 4);
            if (num == 1)
                carArrayNew[j] = 1;
        }
        if ((rand() % 2) == 0) {
            carArrays[i].isRight = true;
        } else {
            carArrays[i].isRight = false;
        }
        memcpy(carArrays[i].array, carArrayNew, sizeof(carArrayNew));
    }
}

//Resets the player to the start and resets the cars (car speed increses)
void nextLevel(void)
{
    setPos(2, 6);
    carsMoveRate += 0.25;
    resetCars ();
}

//Resets the whole game
void resetGame(void)
{
    gameOver = false;
    carsMoveRate = INITIAL_MOVE_RATE;

    setPos(2, 6);
    stepCount = 0;
    resetCars ();
}

//Resets the game for both users using IR 
void resetForBoth(char character)
{
    ir_uart_putc(character);
    ledFlashRate = 0;
    
    pio_output_low(LED_PIO);
    ledState = false;
    resetGame ();
}


