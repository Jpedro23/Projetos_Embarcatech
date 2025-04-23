# 📁 Encapsulamento no Projeto - Raspberry Pi Pico

Este projeto demonstra o uso de **encapsulamento** e **organização modular** em um sistema embarcado com a Raspberry Pi Pico, utilizando arquivos separados para maior clareza, manutenção e reuso.

---

## 🧠 O que é Encapsulamento?

Encapsulamento é o princípio de **separar o código por responsabilidade**, onde cada componente (LED, botão, temperatura) é isolado em seu próprio módulo. Isso traz diversos benefícios:

- ✅ Facilidade de leitura e manutenção  
- ♻️ Reutilização de componentes  
- 🧪 Testes mais simples e isolados  
- 🔒 Evita conflitos de nomes e duplicações

---

## 📦 Estrutura do Projeto

O projeto foi organizado em três partes principais:

```
/include        ← Arquivos de cabeçalho (.h)
/src            ← Código-fonte implementado (.c)
teste_encapsulamento.c ← Arquivo principal (main)
```

---

## 📁 Pasta `include/` - Cabeçalhos `.h`

Essa pasta centraliza os arquivos de **cabeçalho** (headers), que funcionam como **interfaces públicas** entre os arquivos. Eles declaram funções e constantes que serão usadas por outros arquivos `.c`.

📌 Pense neles como **pontes de ligação** entre os arquivos `.c`.

### Exemplo: `include/led.h`
```c
#ifndef LED_H
#define LED_H

void led_init();
void led_on_red();
void led_on_blue();
void led_off();

#endif
```

### Ligação no `CMakeLists.txt`
```cmake
target_include_directories(teste_encapsulamento PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
)
```

> ✅ É necessário adicionar os diretórios `include/` e `src/` para que o compilador encontre os arquivos.

---

## 🔧 Pasta `src/` - Implementações `.c`

A pasta `src` (source) é onde você implementa de fato as funções declaradas nos headers. Cada `.c` é responsável por **executar** a lógica relacionada à sua área.

### Exemplo: `src/led.c`
```c
#include "led.h"
#include "pico/stdlib.h"

#define LED_PIN_RED 13
#define LED_PIN_BLUE 12

void led_init() {
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
}

void led_on_red() {
    gpio_put(LED_PIN_RED, 1);
}

void led_on_blue() {
    gpio_put(LED_PIN_BLUE, 1);
}

void led_off() {
    gpio_put(LED_PIN_RED, 0);
    gpio_put(LED_PIN_BLUE, 0);
}
```

---

## ⚙️ Adicionando os arquivos `.c` no `CMakeLists.txt`

```cmake
add_executable(teste_encapsulamento 
    teste_encapsulamento.c
    src/led.c
    src/botao.c
    src/temperatura.c
    src/calculo.c
)
```

---

## 🧩 Lógica no `main.c`

O `main.c` atua como ponto de entrada do programa. Ele apenas chama funções principais, mantendo a estrutura **limpa** e **simples**.

### Exemplo: `teste_encapsulamento.c`
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "led.h"
#include "temperatura.h"
#include "botao.h"
#include "calculo.h"

int main() {
    stdio_init_all();
    sleep_ms(3000);  // Aguarda reconhecimento da USB

    led_init();
    botao_init();
    temperatura_init();

    printf("Sistema iniciado!
");

    while (true) {
        calculo_init();
        sleep_ms(1000);
    }

    return 0;
}
```

---

## ✅ Benefícios da Organização Modular

- 🔧 **Manutenção facilitada**: mude `led.c` sem afetar os outros módulos
- 🔁 **Reutilização**: use os mesmos arquivos em novos projetos
- 👀 **Clareza**: o `main()` fica mais legível e direto
- 🤝 **Colaboração**: mais fácil para trabalhar em equipe
