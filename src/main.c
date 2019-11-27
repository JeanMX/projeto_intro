/**
 * @file main.c
 *
 * @brief Main function
 */

#include <stdio.h>
#include "main.h"
#include "mcu.h"
#include "serial.h"
#include "keypad.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

/*****************************************
 * Main Function
 *****************************************/

char send_data[256];

// void imprime_linha_coluna(int x, int y){
//     // sprintf(send_data, "Teclado: ");
//     // serial_printf(send_data);
//     sprintf(send_data,"%c\n", buttons_symbols[x -1 ][y - 1]);

//     serial_printf(send_data);
//     mcu_sleep(10);
// }

int main(void) {
    mcu_init();
    keypad_init();
    serial_init();

    for (;;) {
        if (keypad_read(send_data)) {
            serial_printf(send_data);
        }
        mcu_sleep(10);
    }
}
