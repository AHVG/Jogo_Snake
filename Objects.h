#ifndef OBJECTS_H_INCLUDED
#define OBJECTS_H_INCLUDED

// Incluindo o arquivo de cabeçalho Vector
#include "Vector.h"

// CAMPO
#define POINTS_AXE_X 10
#define POINTS_AXE_Y 10
#define F_FRAME_WIDTH 25
#define F_FRAME_HEIGHT 25
// COBRA
#define GROWTH_SPEED 10
#define SNAKE_SIZE 100
#define S_FRAME_WIDTH 25
#define S_FRAME_HEIGHT 25
// MAÇÃ
#define A_FRAME_WIDTH 15
#define A_FRAME_HEIGHT 15

// Estado da cobra e dos botões left e right, respectivamente
enum {S_FORWARD, S_EATING};
enum {PRESS_LEFT, PRESS_RIGHT, NOTHING};

// CAMPO
// Ponto de nascimento da maçã
typedef struct{

    VECTOR position;
    bool live;

}SEED;
// O campo
typedef struct{

    SEED seed[POINTS_AXE_Y][POINTS_AXE_X];         // Armazena os locais de cada semente
    SEED open_seed[POINTS_AXE_Y * POINTS_AXE_X];   // Armazena os locais das sementes abertas (a cobra não está em cima)
    int number_of_open_seed;                       // Número de sementes abertas
    double height;                                 // Altura do campo
    double width;                                  // Largura do campo

    int frame_width;                               // Largura do bit map
    int frame_height;                              // Altura do bit map
    ALLEGRO_BITMAP *image_field;                   // Bit map

}FIELD;

// SNAKE
// Corpo da cobra
typedef struct{

    VECTOR position; // Posiçaõ do corpo
    double size;     // Tamanho do corpo
    double angle;    // Angulo que o corpo faz com a horizontal da tela

}SNAKE_BODY;

// A cobra
typedef struct{

    SNAKE_BODY body[SNAKE_SIZE * GROWTH_SPEED]; // Corpo todo
    VECTOR speed;                               // Velocidade da cobra

    double rotation_speed; // Velocidade angular da velocidade
    int growth_speed;      // Velocidade de crescimento
    int max_size;          // Tamanho máximo da cobra
    int state;             // Estado da cobra (futuramente poderia ser implementado animações)
    int size;              // Tamanho atual da cobra

    int frame_width;            // Comprimento do bit map
    int frame_height;           // altura do bit map
    ALLEGRO_BITMAP *image_head; // Imagem da cabeça
    ALLEGRO_BITMAP *image_body; // Imagem do corpo
    ALLEGRO_BITMAP *image_tail; // Imagem da cauda

}SNAKE;

// MAÇÃ
typedef struct{

    VECTOR position; // Posição da maçã

    double size; // Tamanho da maçã
    bool live;   // Verifica se ela foi comida ou não

    int frame_width;             // Comprimento do bit map
    int frame_height;            // Altura do bit map
    ALLEGRO_BITMAP *image_apple; // Imagem da maçã

}APPLE;

// CONSOLE
typedef struct{

    int c_frame_width;             // Comprimento do bit map do console
    int c_frame_height;            // Altura do bit map do console
    ALLEGRO_BITMAP *image_console; // Imagem do console

    int c_k_frame_width;                          // Comprimento do bit map das teclas
    int c_k_frame_height;                         // Altura do bit map das teclas
    int state_keyboard;                           // Estado das teclas
    ALLEGRO_BITMAP *image_console_keyboard;       // Imagem das teclas despressionadas
    ALLEGRO_BITMAP *image_console_keyboard_right; // Imagem da tecla direita pressionada
    ALLEGRO_BITMAP *image_console_keyboard_left;  // Imagem da tecla esquerda pressionada

}CONSOLE;

// CAMPO_________________________________________________________________________________________________________________________

// INICIALIZAR O FIELD
void f_initialize(FIELD *field, double height, double width, ALLEGRO_BITMAP *f_image);

// ATUALIZAR O ESTADO DOS PONTOS
void f_update(FIELD *field, APPLE apple, SNAKE snake);

// DEBUG DOS PONTOS
void f_draw(FIELD field, int point, ALLEGRO_FONT *font);

// SNAKE__________________________________________________________________________________________________________________________

// INICIALIZAR A SNAKE
void s_initialize(SNAKE *snake, double snake_speed, double snake_rotation_speed, double b_size, ALLEGRO_BITMAP *h_image, ALLEGRO_BITMAP *b_image, ALLEGRO_BITMAP *t_image);

// FUNÇÕES PARA MOVER A SNAKE
void s_forward(SNAKE *snake);
void s_rotation_to_left(SNAKE *snake);
void s_rotation_to_right(SNAKE *snake);

// COLISÃO DA SNAKE COM O PRÓPRIO CORPO
int s_collision(SNAKE snake);

// DETECTANDO SE A SNAKE COMEU A APPLE
void s_eat(SNAKE *snake, APPLE *apple);

// CRESCIMENTO DA SNAKE
void s_grow(SNAKE *snake, APPLE *apple);

// DESENHAR A SNAKE
void s_draw(SNAKE snake);

// MAÇÃ_________________________________________________________________________________________________________________________

// FUNÇÃO PARA INICIALIZAR A APPLE
void a_initialize(APPLE *apple, double a_size, int f_width, int f_height, ALLEGRO_BITMAP *a_image);

// GERAR A APPLE
void a_generate(APPLE *apple, FIELD field);

// DESENHAR A APPLE
void a_draw(APPLE apple);

// CONSOLE______________________________________________________________________________________________________________________

// FUNÇÃO PARA INICIALIZAR O CONSOLE
void c_initialize(CONSOLE *console, int c_width, int c_height, ALLEGRO_BITMAP *c_image, ALLEGRO_BITMAP *c_keyboard, ALLEGRO_BITMAP *c_keyboard_l, ALLEGRO_BITMAP *c_keyboard_r);

// DESENHAR O CONSOLE
void c_draw(CONSOLE console);

// DEBUG________________________________________________________________________________________________________________________

void g_debug(FIELD field, SNAKE snake, APPLE apple, ALLEGRO_FONT *font);

#endif // OBJECTS_H_INCLUDED
