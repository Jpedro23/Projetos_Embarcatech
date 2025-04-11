#include "temperatura.h"
#include "hardware/adc.h"

void temperatura_init(){
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

}

float ler_temperatura(){
    uint16_t raw = adc_read();
    const float conversion = 3.3f / (1 << 12);
    float voltage = raw * conversion;
    return 27 - (voltage - 0.706) / 0.001721;
}