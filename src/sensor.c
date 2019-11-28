#include "sensor.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "utils.h"

#define ADC_BUFFER_SIZE 512
#define OLD_VALUE_WEIGHT 3
#define NEW_VALUE_WEIGHT 1

static uint32_t temperature = 0;
//curto renzo melado
static uint32_t adc_buffer[ADC_BUFFER_SIZE] = {0};

void sensor_init(void){
    MX_DMA_Init();
    MX_ADC_Init();
    HAL_ADC_Start_DMA(&hadc, adc_buffer, ADC_BUFFER_SIZE);
}

float sensor_get_temperature() {
    float current_temperature;
    current_temperature = map(temperature / 100.0, 0, 3.3, 0, 5.0) * 10;
    return current_temperature;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    uint32_t val = 0;

    for (int i = 0; i < ADC_BUFFER_SIZE; i++) {
        val += adc_buffer[i];
    }

    val /= ADC_BUFFER_SIZE;

    temperature = (NEW_VALUE_WEIGHT * val + OLD_VALUE_WEIGHT * temperature)/ (NEW_VALUE_WEIGHT + OLD_VALUE_WEIGHT);
}
