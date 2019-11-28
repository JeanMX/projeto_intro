/**
 * @file serial.c
 *
 * @brief Send data through serial comm.
 *
 * @author Lucas Haug <lucas.haug@thunderatz.org>
 *
 * @date 11/2019
 */

#include <string.h>

#include "serial.h"

#include "usart.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/

void serial_init(void) {
    MX_USART2_UART_Init();
}

void serial_printf(char* data) {
    HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), 100);
}

void serial_newline() {
    char* data = "\n";
    HAL_UART_Transmit(&huart2, (uint8_t*) data, strlen(data), 100);
}
