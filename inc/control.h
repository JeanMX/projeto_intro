#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdbool.h>

#include "main.h"

typedef struct control_config {
    uint16_t temp;
    uint16_t fan_out_speed;
} control_config_t;

void control_init(control_config_t* configuration);

// bool get_configuration(control_config_t configuration);

void control_run(control_config_t* configuration);

void control_stop();

void control_test();

#endif // __CONTROL_H__
