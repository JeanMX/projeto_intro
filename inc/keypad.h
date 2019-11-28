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
 * @brief Read data from keyboard
 *
 * @param value Pointer to keypad vaue read
 *
 * @retval true if success, false otherwise
 */
bool keypad_read(uint8_t* value);

/**
 * @brief Read number from keyboard
 *
 * @retval Number read
 */
uint16_t keypad_read_number();

#endif // __KEYPAD_H__
