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
#include "control.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

typedef enum states {
    INIT,
    CONFIGURATION,
    WAITING_FOR_START,
    RUNNING,
    TESTING,
} state_t;


/*****************************************
 * Main Function
 *****************************************/


int main(void) {
    mcu_init();
    serial_init();
    keypad_init();


    control_config_t configuration;
    state_t state = INIT;
    // state_t state = TESTING;

    for (;;) {
        switch (state) {
            uint8_t command;
            case INIT:
                serial_printf("\tINICIALIZACAO\n");
                control_init(configuration);
                serial_printf("Pressione enter para iniciar ou back para configurar\n");
                state = WAITING_FOR_START;
                break;

            case CONFIGURATION:
                if (get_configuration(configuration)) {
                    serial_printf("Pressione enter para iniciar ou back para reconfigurar\n");
                    state = WAITING_FOR_START;
                }
                break;

            case WAITING_FOR_START:
                if (keypad_read(&command)) {
                    if (command == '*') {
                        serial_printf("\tCONFIGURACAO MANUAL\n");
                        state = CONFIGURATION;
                    } else if (command == '#') {
                        serial_printf("GO\n");
                        state = RUNNING;
                    }
                }
                break;

            case RUNNING:
                control_run(configuration);
                if (keypad_read(&command)) {
                    if (command == '*' || command == '#') {
                        serial_printf("\tDESLIGANDO\n");
                        control_stop();
                        serial_printf("Pressione enter para iniciar ou back para configurar\n");
                        state = WAITING_FOR_START;
                    }
                }
                break;

            case TESTING:
                control_init(configuration);
                control_test();
        }
    }
}
