#include <stdio.h>
#include <stdbool.h>
#include "botao.h"
#include "pico/stdlib.h"


#define BOTAO_PIN 5

void botao_init() {
    gpio_init(BOTAO_PIN);
    gpio_set_dir(BOTAO_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_PIN); // botão com pull-up interno
}

bool botao_pressionado() {
    return !gpio_get(BOTAO_PIN); // retorna true se pressionado (nível baixo)
}
