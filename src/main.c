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
    // sensor_init();
    fans_init();
    rele_init();

    control_config_t configuration;
    configuration.temp = 55;
    configuration.fan_out_speed = 75;

    state_t state = CONFIGURATION;

    for (;;) {
        switch (state) {
            case CONFIGURATION:
                if (get_configuration(configuration)) {
                    state = WAITING_FOR_START;
                }
                break;

            case WAITING_FOR_START:
                uint8_t comand;
                if (keypad_read(&command)) {
                    if (command == '*') {
                        state = CONFIGURATION;
                    } else if (command == '#') {
                        state = RUNNING;
                    }
                }
                break;

            case RUNNING:
                control_run(configuration);
                uint8_t comand;
                if (keypad_read(&command)) {
                    if (command == '*' || command == '#') {
                        state = WAITING_FOR_START;
                    }
                }
                break;
        }
    }
}
