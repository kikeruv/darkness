#include "ripes_system.h"
//cambios de la vicki

#define LED_MATRIX_0_BASE (0xf0000000)
#define LED_MATRIX_0_SIZE (0xdac)
#define LED_MATRIX_0_WIDTH (0x23)
#define LED_MATRIX_0_HEIGHT (0x19)

#define SWITCHES_0_BASE (0xf0000dac)
#define SWITCHES_0_SIZE (0x4)
#define SWITCHES_0_N (0x8)

#define D_PAD_0_BASE (0xf0000db0)
#define D_PAD_0_SIZE (0x10)
#define D_PAD_0_UP_OFFSET (0x0)
#define D_PAD_0_UP (0xf0000db0)
#define D_PAD_0_DOWN_OFFSET (0x4)
#define D_PAD_0_DOWN (0xf0000db4)
#define D_PAD_0_LEFT_OFFSET (0x8)
#define D_PAD_0_LEFT (0xf0000db8)
#define D_PAD_0_RIGHT_OFFSET (0xc)
#define D_PAD_0_RIGHT (0xf0000dbc)

void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void clear_snake(unsigned int x, unsigned int y);
void move_snake(unsigned int x, unsigned int y);
void create_apple();
int check_collision(unsigned int x, unsigned int y);

unsigned int matrizLEDs[FILAS];

void dibujarMatriz() {
    for (int i = 0; i < FILAS; i++) {
        (led_base + i) = matrizLEDs[i];
    }
}

void crecerSerpiente(unsigned int direccion) {
    // Lógica para mover la serpiente y actualizar la matriz de LEDs
    // Se debe manejar la dirección usando las entradas de los interruptores
}

void main() {
    // Inicializar el juego (posiciones iniciales de la serpiente y manzana)
    for (int i = 0; i < FILAS; i++) {
        matrizLEDs[i] = 0x00; // Apagar todos los LEDs al inicio
    }

    while (1) {
        unsigned int entrada =switch_base;

        if (entrada & SW0) {
            // Movimiento hacia la izquierda
            crecerSerpiente(SW0);
        } else if (entrada & SW1) {
            // Movimiento hacia arriba
            crecerSerpiente(SW1);
        } else if (entrada & SW2) {
            // Movimiento hacia la derecha
            crecerSerpiente(SW2);
        } else if (entrada & SW3) {
            // Movimiento hacia abajo
            crecerSerpiente(SW3);
        }

        dibujarMatriz();

        // Agregar un retraso para controlar la velocidad del juego
        for (volatile int i = 0; i < 1000000; i++); // Retraso simple
    }
}