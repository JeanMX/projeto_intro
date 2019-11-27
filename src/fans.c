#include "fans.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f0xx_hal_tim.h"

#define TIM_PERIOD 1000

typedef struct __attribute__((packed)) fan_pin {
    GPIO_TypeDef* port;
    uint16_t pin;
} fan_pin_t;

typedef struct __attribute__((packed)) fan {
    fan_pin_t dir1;
    fan_pin_t dir2;
    TIM_HandleTypeDef* htim;
    uint32_t channel;
} fan_adapter_t;

static fan_adapter_t fans[] = {
    {
        .dir1 = {
            .port = GPIOA,
            .pin = GPIO_PIN_12,
        },
        .dir2 = {
            .port = GPIOA,
            .pin = GPIO_PIN_11,
        },
        .htim = &htim3,
        .channel = TIM_CHANNEL_1,
    },
    {
        .dir1 = {
            .port = GPIOB,
            .pin = GPIO_PIN_12,
        },
        .dir2 = {
            .port = GPIOB,
            .pin = GPIO_PIN_11,
        },
        .htim = &htim3,
        .channel = TIM_CHANNEL_2,
    }
};

void fans_init() {
    MX_TIM3_Init();

    for (int i = 0; i < FANS_AMOUNT; i++) {
        HAL_GPIO_WritePin(fans[i].dir1.port, fans[i].dir1.pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(fans[i].dir2.port, fans[i].dir2.pin, GPIO_PIN_RESET);
    }
}

void fans_set(fan_t fan, uint16_t speed) {
    if (speed <= 0) {
        HAL_TIM_PWM_Stop(fans[fan].htim, fans[fan].channel);
    } else {
        HAL_TIM_PWM_Start(fans[fan].htim, fans[fan].channel);
        __HAL_TIM_SET_COMPARE(fans[i].htim, fans[i].channel, map(speed, 0, 100, 0, TIM_PERIOD));
    }
}
