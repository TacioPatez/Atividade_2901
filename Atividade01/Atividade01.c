#include <stdio.h>
#include "pico/stdlib.h"   // Inclui a biblioteca padrão para funcionalidades básicas como GPIO, temporização e comunicação serial.
#include "hardware/timer.h" // Inclui a biblioteca para gerenciamento de temporizadores de hardware.

#define tempo 3000
#define LED_R 11
#define LED_Y 12
#define LED_G 13

// variavel global definidora do led ativo
uint8_t led_on = 0;

// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {
    // Imprime uma mensagem na saída serial indicando que 3 segundos se passaram.
    printf("3 segundos passaram\n");
    uint8_t led_atual = led_on % 3;
    switch (led_atual) {
    case 0: // se led atual é vermelho
        gpio_put(LED_R, false);
        gpio_put(LED_G, true);
        led_on++;
        break;
    case 1: // se led atual é verde
        gpio_put(LED_G, false);
        gpio_put(LED_Y, true);
        led_on++;
        break;
    case 2: // se led atual é amarelo
        gpio_put(LED_Y, false);
        gpio_put(LED_R, true);
        led_on++;
        break;
    default:
        break;
    }
    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para.
    return true;
}

// Loop infinito que mantém o programa em execução.
int main() {
    // inicialização
    stdio_init_all();

    gpio_init(LED_R);   // Inicializa o LED
    gpio_set_dir(LED_R, GPIO_OUT);  // Configura o led como saida
    gpio_init(LED_Y);   // Inicializa o LED
    gpio_set_dir(LED_Y, GPIO_OUT);  // Configura o led como saida
    gpio_init(LED_G);   // Inicializa o LED
    gpio_set_dir(LED_G, GPIO_OUT);  // Configura o led como saida

    gpio_put(LED_R, true); // inicia com led vermelho ligado

    // Declaração de uma estrutura de temporizador de repetição.
    // Esta estrutura armazenará informações sobre o temporizador configurado.
    struct repeating_timer timer;

    // Configura o temporizador para chamar a função de callback a cada 1 segundo.
    add_repeating_timer_ms(tempo, repeating_timer_callback, NULL, &timer);

    while (true) {
        // Pausa de 10 segundos para reduzir o uso da CPU.
        sleep_ms(10000);
        printf("rotina de repetição\n");
    }

    return 0;
}
