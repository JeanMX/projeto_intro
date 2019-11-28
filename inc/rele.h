#ifndef __RELE_H__
#define __RELE_H__

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "mcu.h"

typedef enum rele_control {
    RELE_CONTROL_OFF,
    RELE_CONTROL_ON,
} rele_control_t;

void rele_init(void);

void rele_control(rele_control_t control_state);

#endif // __RELE_H__
