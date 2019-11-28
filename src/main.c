/**
 * @file main.c
 *
 * @brief Main function
 */

#include <stdio.h>
#include "main.h"
#include "mcu.h"
#include "serial.h"

#include "control.h"
#include "keypad.h"
#include "fans.h"
#include "rele.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

typedef enum states {
    INIT,
    CONFIGURATION,
    WAITING_FOR_START,
    RUNNING
} state_t;


/*****************************************
 * Main Function
 *****************************************/


int main(void) {
    mcu_init();
    serial_init();
    keypad_init();
    rele_init();
    fans_init();
    sensor_init();

    control_config_t configuration;
    state_t state = INIT;

    for (;;) {
        switch (state) {
            uint8_t command;
            case INIT:
                serial_printf("\tINICIALIZACAO\n");
                configuration.temp = 45;
                configuration.fan_out_speed = 75;
                serial_printf("Pressione enter para iniciar ou back para configurar\n");
                state = WAITING_FOR_START;
                break;

            case CONFIGURATION:
                if (get_configuration(configuration)) {
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
                        state = WAITING_FOR_START;
                    }
                }
                break;
        }
    }
}
