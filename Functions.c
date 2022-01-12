#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include "Objects.h"

// Conversão de graus para radianos
#define deg_to_rad(x) (((M_PI) * (x))/(180))

// FUNÇÕES DO CAMPO_____________________________________________________________________________________________________________
void f_initialize(FIELD *field, double height, double width, ALLEGRO_BITMAP *f_image){

    // Defini a distância de uma semente à outra
    double dx = (double)((SCREEN_WIDTH) / POINTS_AXE_X);
    double dy = (double)((SCREEN_HEIGHT) / POINTS_AXE_Y);

    // Defini as caracteristicas do campo
    field->height = height;
    field->width = width;
    field->image_field = f_image;
    field->frame_height = F_FRAME_HEIGHT;
    field->frame_width = F_FRAME_WIDTH;

    // Inicializa todas as sementes
    for(int i = 0; i < POINTS_AXE_Y; i++){
        for(int k = 0; k < POINTS_AXE_X; k++){
            field->seed[i][k].position.x = dx * k + dx/2 + (DELTA_WIDTH)/2;
            field->seed[i][k].position.y = dy * i + dy/2 + (DELTA_HEIGHT)/2;
            field->seed[i][k].live = false;

            field->open_seed[i * POINTS_AXE_Y + k].position.x = 0;
            field->open_seed[i * POINTS_AXE_Y + k].position.y = 0;
            field->open_seed[i * POINTS_AXE_Y + k].live = false;
            field->number_of_open_seed = 0;
        }
    }
}

void f_update(FIELD *field, APPLE apple, SNAKE snake){

    // Distância do corpo da cobra à semente
    double dx;
    double dy;

    // Algoritmo para verificar se a semente está aberta
    if(!apple.live){
        field->number_of_open_seed = 0;
        for(int i = 0; i < POINTS_AXE_Y; i++){
            for(int k = 0; k < POINTS_AXE_X; k++){
                for(int h = 0; h < snake.size; h++){
                    dx = field->seed[i][k].position.x - snake.body[h].position.x;
                    dy = field->seed[i][k].position.y - snake.body[h].position.y;

                    if(pow(snake.body[h].size, 2) > pow(dx, 2) + pow(dy, 2)){
                        field->seed[i][k].live = false;
                        break;
                    }
                    else if(h == snake.size - 1){
                        field->seed[i][k].live = true;

                        field->open_seed[field->number_of_open_seed].position.x = field->seed[i][k].position.x;
                        field->open_seed[field->number_of_open_seed].position.y = field->seed[i][k].position.y;
                        field->open_seed[field->number_of_open_seed].live = true;
                        field->number_of_open_seed++;

                    }
                }
            }
        }
    }
}

void f_draw(FIELD field, int point, ALLEGRO_FONT *font){

    // Desenha o campo
    al_draw_bitmap(field.image_field, DELTA_WIDTH/2, DELTA_HEIGHT/2, 0);

    // Desenha os pontos na tela
    al_draw_textf(font, al_map_rgb(102,102,102), DELTA_WIDTH/2 + SCREEN_WIDTH/2 + 5, DELTA_HEIGHT/2 + SCREEN_HEIGHT/3, ALLEGRO_ALIGN_CENTER,
                  "%d", point);
    al_draw_textf(font, al_map_rgb(255,255,255), DELTA_WIDTH/2 + SCREEN_WIDTH/2, DELTA_HEIGHT/2 + SCREEN_HEIGHT/3, ALLEGRO_ALIGN_CENTER,
                  "%d", point);
}

// FUNÇÕES DA SNAKE_____________________________________________________________________________________________________________
void s_initialize(SNAKE *snake, double snake_speed, double snake_rotation_speed, double b_size, ALLEGRO_BITMAP *h_image, ALLEGRO_BITMAP *b_image, ALLEGRO_BITMAP *t_image){

    // Inicializando atributos globias da snake
    snake->speed.module = snake_speed;
    snake->speed.x = snake_speed;
    snake->speed.y = 0;
    snake->speed.angle = 0;   // A snake faz um ângulo de 0º com a horizontal da tela
    snake->rotation_speed = snake_rotation_speed;
    snake->growth_speed = GROWTH_SPEED;
    snake->max_size = SNAKE_SIZE * GROWTH_SPEED;
    snake->size = 2 * GROWTH_SPEED;
    snake->state = S_FORWARD;
    snake->frame_width = S_FRAME_WIDTH;
    snake->frame_height = S_FRAME_HEIGHT;
    snake->image_head = h_image;
    snake->image_body = b_image;
    snake->image_tail = t_image;

    // Inicializando atributos do corpo da snake
    snake->body[0].size = b_size;
    snake->body[0].angle = 0;
    snake->body[0].position.x = (SCREEN_WIDTH + (DELTA_WIDTH)/2)/2;
    snake->body[0].position.y = (SCREEN_HEIGHT + (DELTA_HEIGHT)/2)/2;

    for(int i = 1; i < SNAKE_SIZE * GROWTH_SPEED; i++){
        snake->body[i].size = b_size - 3;
        snake->body[i].position.x = -10;
        snake->body[i].position.y = -10;
        snake->body[i].angle = 0;
    }

}

void s_forward(SNAKE *snake){

    // Atualiza a posição de cada pedaço da cobra
    for(int i = snake->max_size - 1; i > 0; i--){
        snake->body[i].position.x = snake->body[i - 1].position.x;
        snake->body[i].position.y = snake->body[i - 1].position.y;
        snake->body[i].angle = snake->body[i - 1].angle;
    }

    // Atualiza a posição da cabeça da cobra
    snake->body[0].position.x += snake->speed.x;
    snake->body[0].position.y += snake->speed.y;

    if(snake->body[0].position.x > SCREEN_WIDTH + DELTA_WIDTH/2){
       snake->body[0].position.x = (DELTA_WIDTH)/2;
    }
    else if(snake->body[0].position.x < (DELTA_WIDTH)/2){
        snake->body[0].position.x = SCREEN_WIDTH + DELTA_WIDTH/2;
    }
    if(snake->body[0].position.y > SCREEN_HEIGHT + (DELTA_HEIGHT)/2){
        snake->body[0].position.y = (DELTA_HEIGHT)/2;
    }
    else if(snake->body[0].position.y < (DELTA_HEIGHT)/2){
        snake->body[0].position.y = SCREEN_HEIGHT + (DELTA_HEIGHT)/2;
    }
}

void s_rotation_to_left(SNAKE *snake){

    // Rotaciona a velocidade para esquerda
    snake->speed.angle += snake->rotation_speed;
    if(snake->speed.angle > 359){
        snake->speed.angle = 0;
    }

    snake->body[0].angle = snake->speed.angle;
    snake->speed.y = sin(deg_to_rad(snake->speed.angle)) * snake->speed.module;
    snake->speed.x = cos(deg_to_rad(snake->speed.angle)) * snake->speed.module;
}

void s_rotation_to_right(SNAKE *snake){

    // Rotaciona a velocidade para direita
    snake->speed.angle -= snake->rotation_speed;
    if(snake->speed.angle < 1){
        snake->speed.angle = 360;
    }

    snake->body[0].angle = snake->speed.angle;
    snake->speed.y = sin(deg_to_rad(snake->speed.angle)) * snake->speed.module;
    snake->speed.x = cos(deg_to_rad(snake->speed.angle)) * snake->speed.module;
}

int s_collision(SNAKE snake){

    // Verifica se ocorreu a colisão da cobra
    double dx;
    double dy;
    double dc = snake.body[0].size + snake.body[1].size;

    for(int i = 11; i < snake.size; i++){
        dx = snake.body[0].position.x - snake.body[i].position.x;
        dy = snake.body[0].position.y - snake.body[i].position.y;
        if(pow(dc,2) > pow(dx, 2) + pow(dy, 2)){
            // Caso ocorreu, retorna 1
            return 1;
        }
    }

    // Caso não ocorreu, returna 0
    return 0;
}
void s_eat(SNAKE *snake, APPLE *apple){

    // Distância da cabeça da cobra à maçã
    double dx = snake->body[0].position.x - apple->position.x;
    double dy = snake->body[0].position.y - apple->position.y;

    // Verifica se a cobra comeu a maçã
    if(apple->live){
        if(pow(snake->body[0].size, 2) > pow(dx , 2) + pow(dy , 2)){

            snake->state = S_EATING;
            apple->live = false;
            snake->size += GROWTH_SPEED;
            if(snake->size > snake->max_size){
                snake->size = snake->max_size;
            }
        }
    }
}

void s_draw(SNAKE snake){

    // Desenha a cobra
    al_draw_rotated_bitmap(snake.image_tail, snake.frame_width - 13, snake.frame_height - 13,
                               snake.body[snake.size - 1].position.x, snake.body[snake.size - 1].position.y, deg_to_rad(snake.body[snake.size - 1].angle + 90), 0);
    for(int i = snake.size - GROWTH_SPEED; i > 0; i -= GROWTH_SPEED){
        al_draw_rotated_bitmap(snake.image_body, snake.frame_width - 13, snake.frame_height - 13,
                               snake.body[i - 1].position.x, snake.body[i - 1].position.y, deg_to_rad(snake.body[i - 1].angle + 90), 0);
    }

    al_draw_rotated_bitmap(snake.image_head, snake.frame_width - 13, snake.frame_height - 9,
                           snake.body[0].position.x, snake.body[0].position.y, deg_to_rad(snake.body[0].angle + 90), 0);

}

// MAÇÃ_____________________________________________________________________________________________________________________________________
void a_initialize(APPLE *apple, double a_size, int f_width, int f_height, ALLEGRO_BITMAP *a_image){

    // Inicializa a maçã
    apple->size = a_size;
    apple->live = false;
    apple->frame_width = f_width;
    apple->frame_height = f_height;
    apple->image_apple = a_image;

}

void a_generate(APPLE *apple, FIELD field){

    // Escolhe aleatoriamente o local da maçã dentre os possíveis
    int aux = 0;
    if(!apple->live){
        aux = rand() % field.number_of_open_seed;
        apple->position.x = field.open_seed[aux].position.x;
        apple->position.y = field.open_seed[aux].position.y;
        apple->live = true;
    }
}

void a_draw(APPLE apple){

    // Desenha a maçã
    if(apple.live){
        al_draw_bitmap(apple.image_apple, apple.position.x - 7, apple.position.y - 9, 0);
    }
}

// CONSOLE___________________________________________________________________________________________________________________________________________
void c_initialize(CONSOLE *console, int c_width, int c_height, ALLEGRO_BITMAP *c_image,
                  ALLEGRO_BITMAP *c_keyboard, ALLEGRO_BITMAP *c_keyboard_l, ALLEGRO_BITMAP *c_keyboard_r){

    // Inicializa o console
    console->c_frame_height = c_height;
    console->c_frame_width = c_width;
    console->image_console = c_image;
    console->c_k_frame_height = -1;
    console->c_k_frame_width = -1;
    console->state_keyboard = NOTHING;
    console->image_console_keyboard = c_keyboard;
    console->image_console_keyboard_left = c_keyboard_l;
    console->image_console_keyboard_right = c_keyboard_r;

}

void c_draw(CONSOLE console){

    // Desenha o console
    al_draw_bitmap(console.image_console, 0, 0, 0);

    if(console.state_keyboard == PRESS_LEFT){
        al_draw_bitmap(console.image_console_keyboard_left, WIDTH/2 - 150, DELTA_HEIGHT/2 + SCREEN_HEIGHT, 0);
    }
    else if(console.state_keyboard == PRESS_RIGHT){
        al_draw_bitmap(console.image_console_keyboard_right, WIDTH/2 - 150, DELTA_HEIGHT/2 + SCREEN_HEIGHT, 0);
    }
    else{
        al_draw_bitmap(console.image_console_keyboard, WIDTH/2 - 150, DELTA_HEIGHT/2 + SCREEN_HEIGHT, 0);
    }

}

// DEBUG_____________________________________________________________________________________________________________________________________________
void g_debug(FIELD field, SNAKE snake, APPLE apple, ALLEGRO_FONT *font){

    double size_lin = DELTA_HEIGHT/20;

    al_draw_textf(font, al_map_rgb(255,255,255), DELTA_WIDTH/2, size_lin, 0,
                  "FIELD height %.0lf width %.0lf", field.height, field.width);
    al_draw_textf(font, al_map_rgb(255,255,255), DELTA_WIDTH/2, 2*size_lin, 0,
                  "SNAKE x %.0lf y %.0lf  %d %d",
                  snake.body[0].position.x - DELTA_WIDTH/2, snake.body[0].position.y - DELTA_HEIGHT/2, snake.size, snake.max_size);
    al_draw_textf(font, al_map_rgb(255,255,255), DELTA_WIDTH/2, 3*size_lin, 0,
                  "APPLE x %.0lf y %.0lf", apple.position.x - DELTA_WIDTH/2, apple.position.y - DELTA_HEIGHT/2);

    for(int i = snake.size; i > 0; i --){
        al_draw_filled_circle(snake.body[i - 1].position.x, snake.body[i - 1].position.y, snake.body[i - 1].size, al_map_rgb(255, 0, 255));
    }

    if(apple.live){
        al_draw_filled_circle(apple.position.x, apple.position.y, apple.size, al_map_rgb(255,0,255));
    }

    for(int i = 0; i < POINTS_AXE_Y; i++){
        for(int k = 0; k < POINTS_AXE_X; k++){
            if(field.seed[i][k].live){
                al_draw_filled_circle(field.seed[i][k].position.x,
                                      field.seed[i][k].position.y,
                                      4, al_map_rgb(0,0,255));
            }
            else{
                al_draw_filled_circle(field.seed[i][k].position.x,
                                      field.seed[i][k].position.y,
                                      4, al_map_rgb(255,0,0));
            }
        }
    }
}
