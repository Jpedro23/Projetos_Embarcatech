#include <stdio.h>
#include "pico/stdlib.h"
#include "led.h"
#include "temperatura.h"
#include "botao.h"
#include "calculo.h"


int main()
{
    stdio_init_all();
    sleep_ms(3000);  // Espera o USB ser reconhecido
    
    led_init();
    botao_init();
    temperatura_init();

    printf("Sistema iniciao! \n");

    while (true) {
        calculo_init();
        sleep_ms(1000);
    }
    return 0;
}