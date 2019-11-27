#ifndef __FANS_H__
#define __FANS_H__

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "mcu.h"

typedef enum fans {
    FAN_IN = 0,
    FAN_OUT = 1,
    FANS_AMOUNT = 2,
} fan_t;


/*****************************************
 * Public Function Prototypes
 *****************************************/

/**
 * @brief Initializes fans.
 */
void fans_init(void);

/**
 * @brief Set fan speed
 *
 * @param fan Which fan to control.
 */
void fans_set(fan_t* fan, uint16_t speed);

#endif // __FANS_H__
