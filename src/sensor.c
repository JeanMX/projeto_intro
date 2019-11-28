#include "sensor.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "mcu.h"
#include "utils.h"

#define ADC_BUFFER_SIZE 256
#define OLD_VALUE_WEIGHT 3
#define NEW_VALUE_WEIGHT 1
#define TEMPERATURE_OFFSET 0

static uint32_t adc_buffer[ADC_BUFFER_SIZE] = {0};

static float temperature = 0;

void sensor_init(void){
    MX_DMA_Init();
    MX_ADC_Init();

    mcu_sleep(100);

    HAL_ADC_Start_DMA(&hadc, adc_buffer, ADC_BUFFER_SIZE);
}

float sensor_get_temperature() {
    return map(temperature, 0, 4096, 0, 330) + TEMPERATURE_OFFSET;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    float val = 0;

    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        val += adc_buffer[i];
    }

    val /= ADC_BUFFER_SIZE;

    temperature = (NEW_VALUE_WEIGHT * val + OLD_VALUE_WEIGHT * temperature)/ (NEW_VALUE_WEIGHT + OLD_VALUE_WEIGHT);
}
