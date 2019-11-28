#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "mcu.h"

typedef struct control_config {
    uint16_t temp;
    uint16_t fan_out_speed;
} control_config_t;

bool get_configuration(control_config_t configuration);

void control_run(control_config_t configuration);

void control_stop();

#endif // __CONTROL_H__
