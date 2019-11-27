#include "keypad.h"

#define NUMBER_OF_LINES 4
#define NUMBER_OF_COLUMNS 3

static char buttons_symbols[NUMBER_OF_LINES][NUMBER_OF_COLUMNS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

static pin_t pinos_linha[NUMBER_OF_LINES] = {
    {
        .port = Line1_GPIO_Port,
        .pin = Line1_Pin,
    },
    {
        .port = Line2_GPIO_Port,
        .pin = Line2_Pin,
    },
    {
        .port = Line3_GPIO_Port,
        .pin = Line3_Pin,
    },
    {
        .port = Line4_GPIO_Port,
        .pin = Line4_Pin,
    }
};

static pin_t pinos_coluna[NUMBER_OF_COLUMNS] = {
    {
        .port = Col1_GPIO_Port,
        .pin = Col1_Pin,
    },
    {
        .port = Col2_GPIO_Port,
        .pin = Col2_Pin,
    },
    {
        .port = Col3_GPIO_Port,
        .pin = Col3_Pin,
    }
};


void keypad_init() {
    MX_GPIO_Init();
}

bool keypad_read(char* c) {
    bool pressed = false;
    int linha = 0;
    int coluna = 0;

    for(int i = 0; i < NUMBER_OF_LINES; i++) {

        for (int j = 0; j < NUMBER_OF_LINES; j++) {
            HAL_GPIO_WritePin(pinos_linha[j].port, pinos_linha[j].pin, GPIO_PIN_RESET);
        }
        // HAL_GPIO_WritePin(Line1_GPIO_Port, Line1_Pin, LOW);
        // HAL_GPIO_WritePin(Line2_GPIO_Port, Line2_Pin, LOW);
        // HAL_GPIO_WritePin(Line3_GPIO_Port, Line3_Pin, LOW);
        // HAL_GPIO_WritePin(Line4_GPIO_Port, Line4_Pin, LOW);

        linha = i;
        HAL_GPIO_WritePin(pinos_linha[i].port, pinos_linha[i].pin, GPIO_PIN_SET);

        // switch(ti){
        //     case 3:
        //         HAL_GPIO_WritePin(Line1_GPIO_Port, Line1_Pin, HIGH);
        //         linha = 1;
        //         break;
        //     case 4:
        //         HAL_GPIO_WritePin(Line2_GPIO_Port, Line2_Pin, HIGH);
        //         linha = 2;
        //         break;
        //     case 5:
        //         HAL_GPIO_WritePin(Line3_GPIO_Port, Line3_Pin, HIGH);
        //         linha = 3;
        //         break;
        //     case 6:
        //         HAL_GPIO_WritePin(Line4_GPIO_Port, Line4_Pin, HIGH);
        //         linha = 4;
        //         break;
        // }
        for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
            if(HAL_GPIO_ReadPin(pinos_coluna[j].port, pinos_coluna[j].pin) == GPIO_PIN_SET){
                coluna = j;
                pressed = true;
                break;
            }
            mcu_sleep(10);
        }

        // if(HAL_GPIO_ReadPin(Col1_GPIO_Port, Col1_Pin) == HIGH){
        //     coluna = 1;
        //     pressed = true;
        // }
        // if(HAL_GPIO_ReadPin(Col2_GPIO_Port, Col2_Pin) == HIGH){
        //     coluna = 2;
        //     pressed = true;
        //     // while(HAL_GPIO_ReadPin(Col2_GPIO_Port, Col2_Pin) == HIGH){}
        // }
        // if(HAL_GPIO_ReadPin(Col3_GPIO_Port, Col3_Pin) == HIGH){
        //     coluna = 3;
        //     pressed = true;
        //     // while(HAL_GPIO_ReadPin(Col3_GPIO_Port, Col3_Pin) == HIGH){}
        // }
        // mcu_sleep(10);

        if (pressed) break;
    }

    if (pressed) {
        *c = buttons_symbols[linha][coluna];
    }

    return pressed;
}
