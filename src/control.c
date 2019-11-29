#include "control.h"

#include <stdio.h>
#include <string.h>

#include "serial.h"
#include "fans.h"
#include "rele.h"
#include "sensor.h"

#include "mcu.h"

#define LOOP_TIME 200

static float acao_ant = 0.0;
static float erro_ant = 0.0;

uint16_t calc_temp(uint8_t* bufer, uint8_t size);

void control_init(control_config_t* configuration) {
    fans_init();
    rele_init();
    sensor_init();

    configuration->temp = 35;
    configuration->fan_out_speed = 45;
}

void control_run(control_config_t* configuration) {
    float erro = 0.0;
    float acao = 0.0;
    float kp = 20.0;
    float ki = 0.009;

    rele_control(RELE_CONTROL_ON);


    if(timer_get() > 200){
        timer_reset();
        float actual_temperature = sensor_get_temperature();
        erro = (actual_temperature - (float)configuration->temp);

        acao = acao_ant + kp*(erro-erro_ant)+ ki*(LOOP_TIME/10)*erro;
        //offset and control action limitation
        if(acao > 80.0){
            acao = 80.0;
        }
        else if(acao < 0.0){
            acao = 0.0;
        }
        fans_set(FAN_IN, (uint16_t)(acao + 20));
        fans_set(FAN_OUT, configuration->fan_out_speed);


        char aux_buf[150];
        sprintf(aux_buf, "Temperatura atual: %d ºC\r\nPWM ventilador: %d\r\n", (uint16_t)(actual_temperature*100), (uint16_t)(acao+20));
        serial_printf(aux_buf);

        erro_ant = erro;
        acao_ant = acao;
    }
}

void control_stop() {
    rele_control(RELE_CONTROL_OFF);
    fans_set(FAN_IN, 0);
    fans_set(FAN_OUT, 0);

    acao_ant = 0.0;
    erro_ant = 0.0;
}

void control_test() {
    // for (;;) {
    //     fans_set(FAN_IN, 100);
    //     mcu_sleep(1000);
    //     fans_set(FAN_OUT, 50);
    //     mcu_sleep(1000);
    //     fans_set(FAN_IN, 0);
    //     mcu_sleep(1000);
    //     fans_set(FAN_OUT, 0);
    //     mcu_sleep(3000);
    // }

    // for (;;) {
    //     rele_control(RELE_CONTROL_ON);
    //     mcu_sleep(5000);
    //     rele_control(RELE_CONTROL_OFF);
    //     mcu_sleep(1000);
    // }

    for (;;) {
        if (timer_get() > 500) {
            timer_reset();
            char aux[50];
            sprintf(aux, "Temperatura: %d\r\n", (uint16_t)(sensor_get_temperature()*100));
            serial_printf(aux);
        }
    }
}
