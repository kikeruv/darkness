#include "ripes_system.h"

#define SW0 (0x01) // Control izquierda
#define SW1 (0x02) // Control arriba
#define SW2 (0x04) // Control derecha
#define SW3 (0x08) // Control abajo

volatile unsigned int * led_base = (int) LED_MATRIX_0_BASE;
/
 Un puntero a la
  dirección de memoria donde se encuentra la base 
  de la matriz de LEDs. volatile indica que el valor 
  en esa dirección de memoria puede cambiar en cualquier 
  momento, por lo que el compilador no optimiza
   las lecturas/escrituras./
volatile unsigned int switch_base = (int) SWITCHES_0_BASE;
/
Un puntero a la dirección de memoria de la base de 
los interruptores. Similar al caso anterior, volatile
 garantiza que siempre
 se lea el valor más reciente de los interruptores.*/
#define FILAS 8
#define COLUMNAS 8

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