#ifndef __KeyPad.h__
#define __Keypad.h__

#include "gpio.h"

int keypad_value(int x, int y);

const GPIO_TypeDef* _KEYPAD_COLUMN_GPIO_PORT[] =
{
															GPIOA,
															GPIOA,
															GPIOA,
															GPIOA
};

const uint16_t _KEYPAD_COLUMN_GPIO_PIN[] =
{
															GPIO_PIN_0,
															GPIO_PIN_1,
															GPIO_PIN_2,
															GPIO_PIN_3
};

const GPIO_TypeDef* _KEYPAD_ROW_GPIO_PORT[] =
{
															GPIOA,
															GPIOA,
															GPIOA,
															GPIOA
};

const uint16_t _KEYPAD_ROW_GPIO_PIN[] =
{
															GPIO_PIN_4,
															GPIO_PIN_5,
															GPIO_PIN_6,
															GPIO_PIN_7
};

#endif
