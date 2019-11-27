#include "rele.h"

void rele_init(void){
    rele_control(RELE_CONTROL_OFF);
}

void rele_control(rele_control_t control_state){
    switch (control_state)
    {
    case RELE_CONTROL_ON:
        HAL_GPIO_WritePin(Rele_Pin, GPIO_PIN_SET);
        break;
    case RELE_CONTROL_OFF:
        HAL_GPIO_WritePin(Rele_Pin, GPIO_PIN_RESET);
        break;
    }
}
