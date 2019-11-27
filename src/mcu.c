/**
 * @file mcu.c
 *
 * @brief MCU related
 */

#include <stdint.h>

#include "mcu.h"

#include "gpio.h"
#include "main.h"
#include "stm32f0xx_hal.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/
static uint32_t timer_ticks = 0; /**< Ticks when timer_reset is called */

void mcu_init(void) {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
}

void mcu_sleep(uint32_t ms) {
    HAL_Delay(ms);
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void timer_reset(void) {
    timer_ticks = HAL_GetTick();
}

uint32_t timer_get() {
    return (HAL_GetTick() - timer_ticks);
}
