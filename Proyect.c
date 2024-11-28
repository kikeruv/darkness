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

//#define SWITCHES_0_BASE (0xf0000dac)
//#define SWITCHES_0_SIZE (0x4)
//#define SWITCHES_0_N (0x8)

//direcciones de memoria para los botones dpad
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
int colision_serpiente();
void create_apple();
int colision_manzana();
void perdiste();
void reset();
void add_snake_segment(Pixel tail);

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

                
            }
        }

    }
}
/**
// Prototipos
void set_pixel(unsigned int x, unsigned int y, unsigned int color);
void create_snake(unsigned int x, unsigned int y);
void clear_snake_tail();
void move_snake(unsigned int x, unsigned int y);
void create_apple();
void check_apple();

void main() {
    srand(time(NULL)); // Inicializa la semilla de rand() con el tiempo actual


    unsigned int *up = (unsigned int *)D_PAD_0_UP;
    unsigned int *down = (unsigned int *)D_PAD_0_DOWN;
    unsigned int *left = (unsigned int *)D_PAD_0_LEFT;
    unsigned int *right = (unsigned int *)D_PAD_0_RIGHT;

    unsigned int x = 17;
    unsigned int y = 12;
    
    unsigned int key_up_pressed = 0;
    unsigned int key_down_pressed = 0;
    unsigned int key_left_pressed = 0;
    unsigned int key_right_pressed = 0;

    snake[0] = y * WIDTH + x; // Coordenada inicial de la serpiente
    create_snake(x, y);
    create_apple(); // Crea una manzana al inicio del juego


    while(1) {
        unsigned int prev_x = x;
        unsigned int prev_y = y;

        if(*up == 1 && key_up_pressed == 0) {
            key_up_pressed = 1;
            y -= 1;
            clear_snake(prev_x, prev_y);
            move_snake(x, y);
        }
        if(*down == 1 && key_down_pressed == 0) {
            key_down_pressed = 1;
            y += 1;
            clear_snake(prev_x, prev_y);
            move_snake(x, y);
        }
        if(*left == 1 && key_left_pressed == 0) {
            key_left_pressed = 1;
            x -= 1;
            clear_snake(prev_x, prev_y);
            move_snake(x, y);
        }
        if(*right == 1 && key_right_pressed == 0) {
            key_right_pressed = 1;
            x += 1;
            clear_snake(prev_x, prev_y);
            move_snake(x, y);
        }
        if (*up == 0) key_up_pressed = 0;
        if (*down == 0) key_down_pressed = 0;
        if (*left == 0) key_left_pressed = 0;
        if (*right == 0) key_right_pressed = 0;

        // Comprueba colisión con la manzana
        check_apple();
    }
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
    unsigned int *led_base = (unsigned int *)LED_MATRIX_0_BASE;
    unsigned int offset = x + (y * WIDTH);
    unsigned int *address = led_base + offset;
    *(address) = color;
}

void create_snake(unsigned int x, unsigned int y) {
    set_pixel(x, y, 0x00ff0000); // Rojo
    set_pixel(x + 1, y, 0x00ff0000); // Rojo
    set_pixel(x, y + 1, 0x00ff0000); //led rojo
    set_pixel(x + 1, y + 1, 0x00ff0000); //led rojo
} 

void clear_snake_tail() {
    unsigned int tail_pos = snake[snake_length - 1];
    unsigned int tail_x = tail_pos % WIDTH;
    unsigned int tail_y = tail_pos / WIDTH;
// Limpia el bloque 2x2 de la cola de la serpiente
    set_pixel(tail_x, tail_y, 0x00000000);
    set_pixel(tail_x + 1, tail_y, 0x00000000);
    set_pixel(tail_x, tail_y + 1, 0x00000000);
    set_pixel(tail_x + 1, tail_y + 1, 0x00000000);
}
void move_snake(unsigned int x, unsigned int y) {
    // Mueve el cuerpo de la serpiente
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = y * WIDTH + x; // Actualiza la cabeza

    // Dibujar la nueva posición de la cabeza (2x2)
    create_snake(x, y);
}

void create_apple() {
    do {
        apple_pos = rand() % (WIDTH * HEIGHT);
    } while (apple_pos % WIDTH + 1 >= WIDTH || apple_pos + WIDTH >= WIDTH * HEIGHT);

    unsigned int apple_x = apple_pos % WIDTH;
    unsigned int apple_y = apple_pos / WIDTH;

    // Dibuja la manzana la manzana (2x2) con LEDs verdes
    set_pixel(apple_x, apple_y, 0x0000ff00);      // Verde
    set_pixel(apple_x + 1, apple_y, 0x0000ff00);  // Verde
    set_pixel(apple_x, apple_y + 1, 0x0000ff00);  // Verde
    set_pixel(apple_x + 1, apple_y + 1, 0x0000ff00); // Verde
}

void check_apple() {
    // Coordenadas de la cabeza de la serpiente (2x2)
    unsigned int head_x = snake[0] % WIDTH;
    unsigned int head_y = snake[0] / WIDTH;

    // Coordenadas de la manzana (2x2)
    unsigned int apple_x = apple_pos % WIDTH;
    unsigned int apple_y = apple_pos / WIDTH;

    // Verificar superposición (bloque 2x2)
    if ((head_x < apple_x + 2 && head_x + 2 > apple_x) &&
        (head_y < apple_y + 2 && head_y + 2 > apple_y)) {
        snake_length++; // Incrementar tamaño
        create_apple();  // Generar una nueva manzana
    }
}