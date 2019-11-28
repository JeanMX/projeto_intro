#include "control.h"

#include "keypad.h"
#include "fans.h"
#include "rele.h"

#include "math.h"

uint16_t calc_temp(uint8_t* bufer, uint8_t size);

bool get_configuration (control_congif_t configuration) {
    uint8_t buffer[5];
    uint8_t size = 0;
    while (size < 5) {
        if (keypad_read(&buffer[size])) {
            if (buffer[size] == '#') {
                break;
            } else if (buffer[size] == '*') {
                size = 0;
            } else {
                size++;
            }
        }
    }
    configuration.temp = constrain(calc_temp(buffer, size), 35, 60);

    size = 0;
    while (size == 0) {
        if (keypad_read(buffer)) {
            size++
            switch (buffer[0]) {
                case '1':
                    configuration.fan_out_speed = 30;
                    break;

                case '2':
                    configuration.fan_out_speed = 65;
                    break;

                case '3':
                    configuration.fan_out_speed = 100;
                    break;

                default:
                    size = 0;
                    break;
            }
        }
    }

    return true;
}

void control_run(control_config_t configuration) {

}

uint16_t calc_temp(uint8_t* bufer, uint8_t size) {
    uint16_t value = 0;
    for (int i = 0; i < size; i++) {
        uint16_t aux = 1;
        for (int j = 0; j < (size-i-1); j++) {
            aux *= 10;
        }
        value += buffer[i]*aux;
    }
    return value;
}
