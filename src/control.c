#include "control.h"

#include "keypad.h"
#include "fans.h"
#include "rele.h"

#include "utils.h"

#define LOOP_TIME 200

static float acao_ant = 0.0;
static float erro_ant = 0.0;

uint16_t calc_temp(uint8_t* bufer, uint8_t size);

bool get_configuration (control_config_t configuration) {
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
            size++;
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
    float erro = 0.0;
    float acao = 0.0;
    float kp = 20.0;
    float ki 0.009;

    rele_control(RELE_CONTROL_ON);


    if(timer_get() > 200){
        timer_reset();
        erro = (sensor_get_temperature() - (float)configuration.temp);

        acao = acao_ant + kp*(erro-erro_ant)+ ki*(LOOP_TIME/10)*erro;
        //offset and control action limitation
        if(acao > 80.0){
            acao = 80.0;
        }
        else if(acao < 0.0){
            acao = 0.0;
        }
        fans_set(FAN_IN, acao + 20);
        fan_set(FAN_OUT, configuration.fan_out_speed);

        erro_ant = erro;
        acao_ant = acao;
    }
}

void control_stop() {
    rele_control(RELE_CONTROL_ON);
    fan_set(FAN_IN, 0);
    fans_set(FAN_OUT, 0);

    acao_ant = 0.0;
    erro_ant = 0.0;
}

uint16_t calc_temp(uint8_t* buffer, uint8_t size) {
    uint16_t value = 0;
    for (int i = 0; i < size; i++) {
        uint16_t aux = 1;
        for (int j = 0; j < (size-i-1); j++) {
            aux *= 10;
        }
        value += (buffer[i]-'0')*aux;
    }
    return value;
}