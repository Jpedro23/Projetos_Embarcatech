#include <stdio.h>
#include "pico/stdlib.h"
#include "temperatura.h"
#include "botao.h"
#include "led.h"

void calculo_init(void)
{
    float temp = ler_temperatura();
    printf("\n ---- Novo ciclo... ---- \n");

    printf("Temperatura: %.2f C\n \n", temp);

    
    if (temp < 28.0)
    {
        printf("Hummm friozinho\n");
        led_off(); // Garante que o led esta desligado
        led_on_blue();
        sleep_ms(1000);
        
    }
    
    else if (temp >= 28.0)
    {
        printf("calor duabooo\n");
        led_off(); // Garante que o led esta desligado
        led_on_red();
        sleep_ms(1000);
        
    }

    if (botao_pressionado())
    {
        led_off();
        printf("O botão foi precionado o led foi apagado\n");
    }
}