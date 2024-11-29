#include "ripes_system.h"
#include <stdlib.h> // Para rand() y srand()
#include <time.h>   // Para inicializar la semilla de rand()
//cambios de la vicki

//#define WIDTH (LED_MATRIX_0_WIDTH)
//#define HEIGHT (LED_MATRIX_0_HEIGHT)
#define LED_MATRIX_0_BASE (0xf0000000) //direccion base de la matriz de LEDS
//#define LED_MATRIX_0_SIZE (0xdac)
#define LED_MATRIX_0_WIDTH (0x23)
#define LED_MATRIX_0_HEIGHT (0x19)

<<<<<<< Updated upstream
//#define SWITCHES_0_BASE (0xf0000dac)
//#define SWITCHES_0_SIZE (0x4)
//#define SWITCHES_0_N (0x8)
=======
volatile unsigned int * led_base = (int) LED_MATRIX_0_BASE;
/*
 Un puntero a la
  dirección de memoria donde se encuentra la base 
  de la matriz de LEDs. volatile indica que el valor 
  en esa dirección de memoria puede cambiar en cualquier 
  momento, por lo que el compilador no optimiza
   las lecturas/escrituras./
volatile unsigned int switch_base = (int) SWITCHES_0_BASE;
*/

/*
Un puntero a la dirección de memoria de la base de 
los interruptores. Similar al caso anterior, volatile
 garantiza que siempre
 se lea el valor más reciente de los interruptores.*/
#define FILAS 8
#define COLUMNAS 8
>>>>>>> Stashed changes

//direcciones de memoria para los botones dpadw
#define D_PAD_0_UP (0xf0000db0)
#define D_PAD_0_DOWN (0xf0000db4)
#define D_PAD_0_LEFT (0xf0000db8)
#define D_PAD_0_RIGHT (0xf0000dbc)

//Direccion de memoria de los switches
#define SWITCHES_0_BASE (0xf0000dac) // Dirección base de switches
#define SWITCH_RESET_BIT 0           // Usamos el switch 0 para reiniciar

#define MAX_PIXELS 100 // Cantidad máxima de píxeles o longitud que puede tener la serpiente

// Intervalo de ciclos para cambiar de posición la serpiente
#define SNAKE_MOVE_CYCLES 1200

// Estructura de los segmentos de la serpiente
typedef struct {
    unsigned int x;
    unsigned int y;
} Pixel;

Pixel snake[MAX_PIXELS];       // Arreglo que guarda los píxeles de la serpiente
unsigned int snake_length = 2; // Longitud inicial de la serpiente
int direction = -1;            // Dirección inicial de la serpiente (-1 = sin movimiento)

unsigned int apple_x, apple_y; // Posición de la manzana

// Variables para controlar los ciclos entre movimientos de la serpiente
unsigned int last_move_cycles = 0;
unsigned int total_cycles = 0;

// Funciones
void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake();
void clear_tail(Pixel tail);
void create_apple();
int apple_collision();
int snake_collision();
void add_snake_segment(Pixel tail);
void game_over();
void reset_game();


void main() {
    srand(2); // Inicializa la semilla para números aleatorios

    // Punteros de los botones del D_PAD
    unsigned int *up = (unsigned int *)D_PAD_0_UP;
    unsigned int *down = (unsigned int *)D_PAD_0_DOWN;
    unsigned int *left = (unsigned int *)D_PAD_0_LEFT;
    unsigned int *right = (unsigned int *)D_PAD_0_RIGHT;
    
    // Segmento inicial de la serpiente
    snake[0].x = 17; // Cabeza
    snake[0].y = 12;
    snake[1].x = 17; // Cola
    snake[1].y = 12;

    //Segmento de la serpiente
    snake[0].x = 17; // Cabeza
    snake[0].y = 12;
    snake[1].x = 17; // Cola
    snake[1].y = 12;

    //Creamos la serpiente y la manzana mandando a llamar a las funciones
    create_snake();
    create_apple();

    //Corazon(Ciclo principal)
    while (1){
        total_cycles++;

        //Direccion conforme el D_PAD
         if (*up == 1 && direction != 1) {
            direction = 0;
        } else if (*down == 1 && direction != 0) {
            direction = 1;
        } else if (*left == 1 && direction != 3) {
            direction = 2;
        } else if (*right == 1 && direction != 2) {
            direction = 3;
        }

        //verficamos si es tiempo de mover a la serpiente
        if (total_cycles - last_move_cycles >= SNAKE_MOVE_CYCLES) {
            last_move_cycles = total_cycles;

            //Guardamos la posicion de la cola
            if (direction != -1) {
                Pixel tail = snake[snake_length - 1]; 

                // Desplaza todos los segmentos de la serpiente
                for (int i = snake_length - 1; i > 0; i--) {
                    snake[i] = snake[i - 1];
                }

                // Actualiza la posicion de la cabeza
                switch (direction) {
                    case 0: snake[0].y -= 2; break; // Arriba
                    case 1: snake[0].y += 2; break; // Abajo
                    case 2: snake[0].x -= 2; break; // Izquierda
                    case 3: snake[0].x += 2; break; // Derecha
                }

                //Verificamos las colisiones
                if (snake[0].x < 0 || snake[0].x >= LED_MATRIX_0_WIDTH || snake[0].y < 0 || snake[0].y >= LED_MATRIX_0_HEIGHT || snake_collision()) {
                    game_over(); // Termina el juego si hay colisión
                }

                // Verifica la colision de la mazana
                if (apple_collision()) {
                    create_apple();
                    add_snake_segment(tail);
                } else {
                    clear_tail(tail); // Borramos la cola si no come
                }

                create_snake(); // Volvemos a actualizar la serpiente
            }
        }

    }
}

//Empezamos con las funciones:

//  Funciones pare verificar si choca la cabeza
int snake_collision() {
    for (unsigned int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

// Borra la cola de la serpiente
void clear_tail(Pixel tail) {
    set_pixel(tail.x, tail.y, 0x00000000); // Apaga el LED
    set_pixel(tail.x + 1, tail.y, 0x00000000);
    set_pixel(tail.x, tail.y + 1, 0x00000000);
    set_pixel(tail.x + 1, tail.y + 1, 0x00000000);
}

// Dibuja la serpiente en la matriz de LEDs
void create_snake() {
    for (unsigned int i = 0; i < snake_length; i++) {
        set_pixel(snake[i].x, snake[i].y, 0x00ff0000); // Rojo
        set_pixel(snake[i].x + 1, snake[i].y, 0x00ff0000);
        set_pixel(snake[i].x, snake[i].y + 1, 0x00ff0000);
        set_pixel(snake[i].x + 1, snake[i].y + 1, 0x00ff0000);
    }
}

// Función para encender un LED en una posición específica
void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
    unsigned int *led_base = (unsigned int *)LED_MATRIX_0_BASE;
    unsigned int offset = x + (y * LED_MATRIX_0_WIDTH);
    *(led_base + offset) = color;
}

// Crea una nueva manzana en una posición aleatoria
void create_apple() {
    apple_x = (rand() % (LED_MATRIX_0_WIDTH / 2)) * 2 + 1;
    apple_y = (rand() % (LED_MATRIX_0_HEIGHT / 2)) * 2;

    for (unsigned int i = 0; i < snake_length; i++) {
        if (apple_x == snake[i].x && apple_y == snake[i].y) {
            create_apple(); // Evita colocar la manzana en el cuerpo
            return;
        }
    }

    set_pixel(apple_x, apple_y, 0x0000ff00); // Verde para la manzana
    set_pixel(apple_x + 1, apple_y, 0x0000ff00);
    set_pixel(apple_x, apple_y + 1, 0x0000ff00);
    set_pixel(apple_x + 1, apple_y + 1, 0x0000ff00);
}

// Verifica si la cabeza colisiona con la manzana
int apple_collision() {
    return snake[0].x == apple_x && snake[0].y == apple_y;
}

// Agrega una parte a la serpiente
void add_snake_segment(Pixel tail) {
    if (snake_length < MAX_PIXELS) {
        snake[snake_length] = tail;
        snake_length++;
    }
}

// Reinicia todas las variables del juego
void reset_game() {
    snake_length = 2;
    direction = -1;
    // Cabeza
    snake[0].x = 17; 
    snake[0].y = 12;
    // Cola
    snake[1].x = 17; 
    snake[1].y = 12;

    last_move_cycles = 0;
    total_cycles = 0;

    create_snake();
    create_apple();
}

// Se termina el juego y se reinicia con el switch 0
void game_over() {
    for (unsigned int x = 0; x < LED_MATRIX_0_WIDTH; x++) {
        for (unsigned int y = 0; y < LED_MATRIX_0_HEIGHT; y++) {
            set_pixel(x, y, 0x00000000); // Apaga todos los LEDs
        }
    }

    unsigned int *switches = (unsigned int *)SWITCHES_0_BASE;

    // Espera a que el switch de reinicio sea activado
    while (!(*switches & (1 << SWITCH_RESET_BIT))) {
       
    }

    reset_game(); // Funcion para reinicia el juego
}