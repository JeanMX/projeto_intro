/**
 * @file main.c
 *
 * @brief Main function
 */

#include <stdio.h>
#include "mcu.h"
#include "serial.h"
#include "gpio.h"
#include "main.h"
/*****************************************
 * Private Constant Definitions
 *****************************************/

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define NUMBER_OF_LINES 4
#define NUMBER_OF_COLUMNS 3
/*****************************************
 * Main Function
 *****************************************/

char send_data[256];
static char buttons_symbols[NUMBER_OF_LINES][NUMBER_OF_COLUMNS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};
void imprime_linha_coluna(int x, int y){
    // sprintf(send_data, "Teclado: ");
    // serial_printf(send_data);
    sprintf(send_data,"%c\n", buttons_symbols[x -1 ][y - 1]);
    
    serial_printf(send_data);
    mcu_sleep(10);
}

int main(void) {
    mcu_init();
    serial_init();

    int linha;

    for (;;) {
        for(int ti = 3; ti < 7; ti++){
            HAL_GPIO_WritePin(Line1_GPIO_Port, Line1_Pin, LOW);
            HAL_GPIO_WritePin(Line2_GPIO_Port, Line2_Pin, LOW);
            HAL_GPIO_WritePin(Line3_GPIO_Port, Line3_Pin, LOW);
            HAL_GPIO_WritePin(Line4_GPIO_Port, Line4_Pin, LOW);
            switch(ti){
                case 3:
                    HAL_GPIO_WritePin(Line1_GPIO_Port, Line1_Pin, HIGH);
                    linha = 1;
                    break;
                case 4:
                    HAL_GPIO_WritePin(Line2_GPIO_Port, Line2_Pin, HIGH);
                    linha = 2;
                    break;
                case 5:
                    HAL_GPIO_WritePin(Line3_GPIO_Port, Line3_Pin, HIGH);
                    linha = 3;
                    break;
                case 6:
                    HAL_GPIO_WritePin(Line4_GPIO_Port, Line4_Pin, HIGH);
                    linha = 4;
                    break;
            }
            if(HAL_GPIO_ReadPin(Col1_GPIO_Port, Col1_Pin) == HIGH){
                imprime_linha_coluna(linha, 1);
                while(HAL_GPIO_ReadPin(Col1_GPIO_Port, Col1_Pin) == HIGH){}
            }
            if(HAL_GPIO_ReadPin(Col2_GPIO_Port, Col2_Pin) == HIGH){
                imprime_linha_coluna(linha, 2);
                while(HAL_GPIO_ReadPin(Col2_GPIO_Port, Col2_Pin) == HIGH){}
            }
            if(HAL_GPIO_ReadPin(Col3_GPIO_Port, Col3_Pin) == HIGH){
                imprime_linha_coluna(linha, 3);
                while(HAL_GPIO_ReadPin(Col3_GPIO_Port, Col3_Pin) == HIGH){}
            }
            mcu_sleep(10);
        }
    }
}
