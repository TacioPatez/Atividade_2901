#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes.

#define tempo 3000
#define LED_B 11
#define LED_R 12
#define LED_G 13
#define BUTTON 5

// variavel global definidora para verificar se os leds estao ativos
bool led_active = false; 

// Função de callback para desligar o LED G após o tempo programado.
int64_t turn_off_ledG(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN para nível baixo.
    gpio_put(LED_G, false);

    // Atualiza o estado de 'led_active' para falso, indicando que o LED está desligado.
    led_active = false;

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

// Função de callback para desligar o LED R após o tempo programado.
int64_t turn_off_ledR(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN para nível baixo.
    gpio_put(LED_R, false);

    // Ativa temporizador para novo led
    add_alarm_in_ms(tempo, turn_off_ledG, NULL, false);

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}


// Função de callback para desligar o LED B após o tempo programado.
int64_t turn_off_ledB(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN para nível baixo.
    gpio_put(LED_B, false);

    // Ativa temporizador para novo led
    add_alarm_in_ms(tempo, turn_off_ledR, NULL, false);

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

int main()
{
    // inicialização
    stdio_init_all();

    gpio_init(LED_B);   // Inicializa o LED
    gpio_set_dir(LED_B, GPIO_OUT);  // Configura o led como saida
    gpio_init(LED_R);   // Inicializa o LED
    gpio_set_dir(LED_R, GPIO_OUT);  // Configura o led como saida
    gpio_init(LED_G);   // Inicializa o LED
    gpio_set_dir(LED_G, GPIO_OUT);  // Configura o led como saida

    gpio_init(BUTTON);  // Inicializa Botao
    gpio_set_dir(BUTTON, GPIO_IN);  // Configura pino como entrada
    gpio_pull_up(BUTTON);   // Habilita Pull Up interno

    while (true) {
        if (gpio_get(BUTTON) == 0 && !led_active) {
            //debounce
            sleep_ms(50);
            // Verifica novamente o estado do botão após o debounce.
            if (gpio_get(BUTTON) == 0) {
                // Liga os LEDs
                gpio_put(LED_B, true);
                gpio_put(LED_R, true);
                gpio_put(LED_G, true);

                // Define 'led_active' como true para indicar que os LEDs estão acesos.
                led_active = true;

                // Agenda um alarme para começar a desligar os LEDs após 3 segundos (3000 ms).
                // A função 'turn_off_ledB' será chamada após esse tempo.
                add_alarm_in_ms(tempo, turn_off_ledB, NULL, false);
            }
        }
    }
}
