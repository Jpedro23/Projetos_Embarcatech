#include "led.h"
#include "pico/stdlib.h"

#define LED_PIN_red 13
#define LED_PIN_blue 12

void led_init()
{
    gpio_init(LED_PIN_red);
    gpio_set_dir(LED_PIN_red, GPIO_OUT);
    gpio_init(LED_PIN_blue);
    gpio_set_dir(LED_PIN_blue, GPIO_OUT);
}

void led_on_red()
{
    gpio_put(LED_PIN_red, 1);
}

void led_on_blue()
{
    gpio_put(LED_PIN_blue, 1);
}

void led_off()
{
    gpio_put(LED_PIN_red, 0);
    gpio_put(LED_PIN_blue, 0);
}
