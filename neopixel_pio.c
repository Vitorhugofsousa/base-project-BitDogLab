#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"


// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

const uint botao_a = 5;
const uint botao_b = 6;

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Função para converter a posição do matriz para uma posição do vetor.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

//Funcao para desenhar a matriz
void desenhaMatriz(int matriz[5][5][3], int tempo_ms, float intensidade){
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
    }
  }
    npWrite();
    sleep_ms(tempo_ms);

}

int led_blackout(){

double apagar_leds[25] = {0.0, 0.0, 0.0, 0.0, 0.0,           //Apagar LEDs da matriz
                          0.0, 0.0, 0.0, 0.0, 0.0, 
                          0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0};
}

    int animacao_tecla_sete(){
        
        int frame1[5][5][3] = {
            {{246, 255, 0}, {6, 0, 158}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{6, 0, 158}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
        };
            desenhaMatriz(frame1, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame2[5][5][3] = {
            {{0, 0, 0}, {246, 255, 0}, {0, 2, 255}, {0, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {0, 2, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {246, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
        };
            desenhaMatriz(frame2, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame3[5][5][3] = {
            {{0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 2, 255}, {0, 0, 0}},
            {{0, 0, 0}, {246, 255, 0}, {0, 2, 255}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {246, 255, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {246, 255, 0}, {246, 255, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {246, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
        };
            desenhaMatriz(frame3, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame4[5][5][3] = {
            {{0, 2, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 255}},
            {{246, 255, 0}, {0, 2, 255}, {246, 255, 0}, {0, 2, 255}, {246, 255, 0}},
            {{0, 0, 0}, {255, 0, 0}, {246, 255, 0}, {255, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}},
            {{0, 0, 0}, {246, 255, 0}, {0, 0, 0}, {246, 255, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame4, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame5[5][5][3] = {
            {{0, 2, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 255}},
            {{246, 255, 0}, {0, 2, 255}, {246, 255, 0}, {0, 2, 255}, {246, 255, 0}},
            {{0, 0, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {0, 0, 0}},
            {{246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}},
            {{0, 0, 0}, {246, 255, 0}, {0, 0, 0}, {246, 255, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame5, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame6[5][5][3] = {
            {{0, 2, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 2, 255}},
            {{246, 255, 0}, {0, 2, 255}, {246, 255, 0}, {0, 2, 255}, {246, 255, 0}},
            {{0, 0, 0}, {255, 0, 0}, {246, 255, 0}, {255, 0, 0}, {0, 0, 0}},
            {{246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}, {246, 255, 0}},
            {{0, 0, 0}, {246, 255, 0}, {0, 0, 0}, {246, 255, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame6, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame7[5][5][3] = {
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 2, 255}},
            {{0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 2, 255}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {255, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {246, 255, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 0, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame7, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
        int frame8[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 2, 255}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {255, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {246, 255, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame8, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia


        int frame9[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {246, 255, 0}}
        };
            desenhaMatriz(frame9, 500, 0.2); //chamando a funcao para desenhar a matriz e passando os parametros de referencia

            int frame0[5][5][3] = {
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
        };
        desenhaMatriz(frame0, 500, 0.5); //chamando a funcao para desenhar a matriz e passando os parametros de referencia
    }

int main() {
  stdio_init_all();// Inicializa entradas e saídas.
  npInit(LED_PIN);// Inicializa matriz de LEDs NeoPixel.

  //iniciando o botão A da bitdogLAB
  gpio_init(botao_a);
  gpio_set_dir(botao_a, GPIO_IN);
  gpio_pull_up(botao_a);
  //iniciando o botão B da bitdogLAB
  gpio_init(botao_b);
  gpio_set_dir(botao_b, GPIO_IN);
  gpio_pull_up(botao_b);



bool botao_pressionado = true;
while (1){
      while (gpio_get(botao_b)){
      bool  botao_atual = gpio_get(botao_b);
        if (botao_atual && !botao_pressionado){
          animacao_tecla_sete();
          botao_pressionado = true;
        }else if (!botao_atual) { // Adicionado para detectar quando o botão é solto
                  botao_pressionado = false;
        }
        sleep_ms(10);
}
                  botao_pressionado = false;
}


  
}
