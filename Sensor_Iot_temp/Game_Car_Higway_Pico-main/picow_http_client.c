/**
 * Cliente HTTP para Raspberry Pi Pico W
 * Envia comandos "LEFT" ou "RIGHT" para um servidor Flask via GET
 */
#include <stdio.h>
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "lwip/altcp_tls.h"
#include "example_http_client_util.h"
#include "lwipopts_examples_common.h"
#include "lwipopts.h"
#include "mbedtls_config_examples_common.h"
#include "mbedtls_config.h"
#include "hardware/adc.h"

// ======= CONFIGURAÇÕES ======= //
#define HOST "192.168.185.71"  // Substitua pelo IP do servido"r
#define PORT 5000
#define INTERVALO_MS 50
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 6
#define LED_BLUE 12
#define LED_RED 13
// ============================= //

// Função para ler a temperatura do sensor dentro do rp2040
float ler_temperatura()
{
    uint16_t raw = adc_read();
    const float conversion = 3.3f / (1 << 12);
    float voltage = raw * conversion;
    return 27 - (voltage - 0.706) / 0.001721;
}


int main() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    gpio_init(BUTTON_LEFT);
    gpio_set_dir(BUTTON_LEFT, GPIO_IN);
    gpio_pull_up(BUTTON_LEFT);

    gpio_init(BUTTON_RIGHT);
    gpio_set_dir(BUTTON_RIGHT, GPIO_IN);
    gpio_pull_up(BUTTON_RIGHT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);



    printf("\nIniciando cliente HTTP...\n");

    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando a %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, 
                                           CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão Wi-Fi\n");
        return 1;
    }

    printf("Conectado! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    while (1) {

        // Lê a temperatura a cada ciclo
        float temperature = ler_temperatura();

        // Formata o path com a temperatura
        char temp_path[128];
        snprintf(temp_path, sizeof(temp_path), "/temperature?value=%.2f", temperature);
        // A cada leitura um sinal de luz azul vai ser mostrado na placa
            gpio_put(LED_BLUE,1);
            sleep_ms(100);
            gpio_put(LED_BLUE,0);

        const char* path = NULL;

/*         if (gpio_get(BUTTON_LEFT) == 0) {
            path = "/left";
            gpio_put(LED_BLUE,1);
            sleep_ms(100);
            gpio_put(LED_BLUE,0);
        }
*/
        // Se algum botão for pressionado, priorize o path correspondente
        if(path == NULL){
            path = temp_path; // Se nenhum botão for pressionado, envia a temperatura
            sleep_ms(1000);
        }

        if (path != NULL) {
            EXAMPLE_HTTP_REQUEST_T req = {0};
            req.hostname = HOST;
            req.url = path;
            req.port = PORT;
            req.headers_fn = http_client_header_print_fn;
            req.recv_fn = http_client_receive_print_fn;

            printf("Enviando comando: %s\n", path);
            int result = http_client_request_sync(cyw43_arch_async_context(), &req);

            if (result == 0) {
                printf("Comando enviado com sucesso!\n");
            } else {
                printf("Erro ao enviar comando: %d\n", result);
            }

            sleep_ms(100);  // Evita múltiplos envios rápidos
        }

        sleep_ms(INTERVALO_MS);
    }
}
