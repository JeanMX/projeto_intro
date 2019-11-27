#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "mcu.h"

typedef struct __attribute__((packed)) pin {
    GPIO_TypeDef* port;
    uint16_t pin;
} pin_t;


/*****************************************
 * Public Function Prototypes
 *****************************************/

/**
 * @brief Initializes serial comm.
 */
void keypad_init(void);

/**
 * @brief Send data through serial comm.
 *
 * @param data Data to be sent.
 */
bool keypad_read(char* c);

#endif // __KEYPAD_H__
