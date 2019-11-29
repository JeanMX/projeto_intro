/**
 * @file main.c
 *
 * @brief Main function
 */

#include <stdio.h>

#include "main.h"
#include "mcu.h"
#include "utils.h"

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
                serial_newline();
                serial_printf("------INICIALIZACAO------\r\n");
                control_init(&configuration);
                serial_printf("Pressione enter para iniciar ou back para configurar\r\n");
                state = WAITING_FOR_START;
                break;

            case CONFIGURATION:
                serial_printf ("Digite a temperatura desejada\r\n");
                configuration.temp = keypad_read_number();
                configuration.temp = constrain(configuration.temp, 30, 60);
                char str_aux[50];
                sprintf(str_aux, "Temperatura escolhida: %d ºC\r\n", configuration.temp);
                serial_printf(str_aux);
                serial_newline();
                serial_printf("Escolha a potencia do ventilador:\r\n\t1 - BAIXA\r\n\t2 - MEDIA\r\n\t3 - ALTA\r\n");
                bool valido = false;
                while (!valido) {
                    if (keypad_read(&command)){
                        switch (command) {
                            case '1':
                                configuration.fan_out_speed = 45;
                                serial_printf("Potencia escolhida: BAIXA\r\n");
                                valido = true;
                                break;

                            case '2':
                                configuration.fan_out_speed = 75;
                                serial_printf("Potencia escolhida: MEDIA\r\n");
                                valido = true;
                                break;

                            case '3':
                                configuration.fan_out_speed = 100;
                                serial_printf("Potencia escolhida: ALTA\r\n");
                                valido = true;
                                break;
                        }
                    }
                }
                serial_newline();
                serial_printf("Pressione enter para iniciar ou back para reconfigurar\r\n");
                state = WAITING_FOR_START;
                break;

            case WAITING_FOR_START:
                if (keypad_read(&command)) {
                    if (command == '*') {
                        serial_newline();
                        serial_printf("------CONFIGURACAO MANUAL------\r\n");
                        state = CONFIGURATION;
                    } else if (command == '#') {
                        serial_newline();
                        serial_printf("------EM FUNCIONAMENTO------\r\n");
                        state = RUNNING;
                    }
                }
                break;

            case RUNNING:
                control_run(&configuration);
                if (keypad_read(&command)) {
                    if (command == '*' || command == '#') {
                        serial_newline();
                        serial_printf("------DESLIGANDO------\r\n");
                        control_stop();
                        serial_printf("Pressione enter para iniciar ou back para configurar\r\n");
                        state = WAITING_FOR_START;
                    }
                }
                break;

            case TESTING:
                control_init(&configuration);
                control_test();
        }
    }
}
