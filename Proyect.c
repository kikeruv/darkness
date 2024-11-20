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

unsigned int apple_x, apple_y;

void main() {
    srand(1); // Inicializa la semilla de rand() con un valor fijo para consistencia

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
        if(*up == 0) {
            key_up_pressed = 0;
        }
        if(*down == 0) {
            key_down_pressed = 0;
        }
        if(*left == 0) {
            key_left_pressed = 0;
        }
        if(*right == 0) {
            key_right_pressed = 0;
        }

        // Comprueba la colisión con la manzana
        if (check_collision(x, y)) {
            create_apple(); // Crea una nueva manzana si hay colisión
        }
    }
}

void set_pixel(unsigned int x, unsigned int y, unsigned int color) {
    unsigned int *led_base = (unsigned int *)LED_MATRIX_0_BASE; 
    unsigned int *address = 0;
    unsigned int offset = 0;
    offset = x + (y * LED_MATRIX_0_WIDTH);
    address = led_base + offset;
    *(address) = color;
}

void create_snake(unsigned int x, unsigned int y) {
    set_pixel(x, y, 0x00ff0000);
    set_pixel(x + 1, y, 0x00ff0000);
    set_pixel(x, y + 1, 0x00ff0000);
    set_pixel(x + 1, y + 1, 0x00ff0000);
}

void clear_snake(unsigned int x, unsigned int y) {
    set_pixel(x, y, 0x00000000);
    set_pixel(x + 1, y, 0x00000000);
    set_pixel(x, y + 1, 0x00000000);
    set_pixel(x + 1, y + 1, 0x00000000);
}

void move_snake(unsigned int x, unsigned int y) {
    create_snake(x, y);
}

void create_apple() {
    apple_x = rand() % (LED_MATRIX_0_WIDTH - 1);
    apple_y = rand() % (LED_MATRIX_0_HEIGHT - 1);

    // Asegura que la manzana esté dentro de los límites de la matriz
    if (apple_x < 1) apple_x = 1;
    if (apple_y < 1) apple_y = 1;

    // Dibuja la manzana
    set_pixel(apple_x, apple_y, 0x0000ff00);      // Verde
    set_pixel(apple_x + 1, apple_y, 0x0000ff00);  // Verde
    set_pixel(apple_x, apple_y + 1, 0x0000ff00);  // Verde
    set_pixel(apple_x + 1, apple_y + 1, 0x0000ff00); // Verde
}

int check_collision(unsigned int x, unsigned int y) {
    // Comprueba si la cabeza de la serpiente (x, y) está en la misma posición que la manzana
    if ((x == apple_x && y == apple_y) || 
        (x + 1 == apple_x && y == apple_y) || 
        (x == apple_x && y + 1 == apple_y) || 
        (x + 1 == apple_x && y + 1 == apple_y)) {
        return 1; // Hay colisión
    }
    return 0; // No hay colisión
}