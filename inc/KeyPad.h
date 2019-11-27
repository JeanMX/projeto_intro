#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "mcu.h"


/*****************************************
 * Public Function Prototypes
 *****************************************/

/**
 * @brief Initializes keypad
 */
void keypad_init(void);

/**
 * @brief Send data through serial comm.
 *
 * @param c Pointer to store character
 */
bool keypad_read(char* c);

#endif // __KEYPAD_H__
