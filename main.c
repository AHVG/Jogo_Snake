#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <time.h>
#include <math.h>
#include "Objects.h"

int main()
{
    //Variáveis
    bool end = false;     // Indica o fim do jogo
	bool draw = true;     // Indica quando é que a máquina vai desenhar os objetos do jogo
	bool update = false;  // Atualiza toda a lógica do jogo
	bool debug = false;   // Ativa o modo debug
	const int FPS = 60;   // Frames por segundo

	//Objetos
    FIELD field;          // O bjeto campo
    SNAKE snake;          // Objeto cobra (player)
    APPLE apple;          // Objeto maçã (ponto)
    CONSOLE console;      // Objeto carcaça do console (estética)

	//Variáveis do Allegro5
	ALLEGRO_DISPLAY *display = NULL;          // Tela de jogo
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;  // Lista de eventos
	ALLEGRO_TIMER *timer = NULL;              // Timer do jogo
    ALLEGRO_KEYBOARD_STATE keyState;          // Variável que armazena o estado da tecla
    ALLEGRO_BITMAP *b_snake_head = NULL;      // Bit map da cabeça da cobra
    ALLEGRO_BITMAP *b_snake_body = NULL;      // Bit map do corpo da cobra
    ALLEGRO_BITMAP *b_snake_tail = NULL;      // Bit map da cauda da cobra
    ALLEGRO_BITMAP *b_field = NULL;           // Bit map do campo
    ALLEGRO_BITMAP *b_apple = NULL;           // Bit map da maçã
    ALLEGRO_BITMAP *b_console = NULL;         // Bit map do console (carcaça)
    ALLEGRO_BITMAP *b_console_keyboard;       // Bit map do teclado sem ser pressionado
    ALLEGRO_BITMAP *b_console_keyboard_left;  // Bit map dp telcado com a tecla esquerda pressionada
    ALLEGRO_BITMAP *b_console_keyboard_right; // Bit map dp telcado com a tecla direita pressionada


    // Inicializações
    if(!al_init())										// Inicializando o allegro5
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			// Criando a tela do jogo

	if(!display)
		return -1;

	al_init_primitives_addon(); // Inicializando as primitivas
	al_install_keyboard();      // Inicializando o teclado
    al_init_font_addon();       // Inicializando a fonte
	al_init_ttf_addon();        // Inicializando alguma coisa
    al_init_image_addon();      // Inicializando alguma coisa

	ALLEGRO_FONT *font1 = al_load_font("Fonts/arial.ttf", 10, 0);  // Carregando a fonte
    ALLEGRO_FONT *font2 = al_load_font("Fonts/arial.ttf", 100, 0); // Carregando a segunda fonte

    b_field = al_load_bitmap("Images/field.png");               // Carregando o bit map do campo
    f_initialize(&field, SCREEN_HEIGHT, SCREEN_WIDTH, b_field); // Inicializando o campo

	b_snake_head = al_load_bitmap("Images/snake_head.png");          // Carregando o bit map da cabeça da cobra
	al_convert_mask_to_alpha(b_snake_head, al_map_rgb(255, 0, 255)); // Tornando-lhe o fundo transparente
    b_snake_body = al_load_bitmap("Images/snake_body.png");          // Carregando o bit map do corpo da cobra
	al_convert_mask_to_alpha(b_snake_body, al_map_rgb(255, 0, 255)); // Tornando-lhe o fundo transparente
    b_snake_tail = al_load_bitmap("Images/snake_tail.png");          // Carregabdo o bit map da cauda da cobra
	al_convert_mask_to_alpha(b_snake_tail, al_map_rgb(255, 0, 255)); // Tornando-lhe o fundo transparente
	s_initialize(&snake, 2, 4, 11, b_snake_head, b_snake_body, b_snake_tail); // Inicializando a cobra

    b_apple = al_load_bitmap("Images/apple.png");                     // Carregando o bit map da maçã
	al_convert_mask_to_alpha(b_apple, al_map_rgb(255, 0, 255));       // Tornando-lhe o fundo transparente
    a_initialize(&apple, 5, A_FRAME_WIDTH , A_FRAME_HEIGHT, b_apple); // Inicializando a maçã

    b_console = al_load_bitmap("Images/console.png");                               // Carregando o bit map do console
	al_convert_mask_to_alpha(b_console, al_map_rgb(255, 0, 255));                   // Tornando-lhe o fundo transparente
    b_console_keyboard = al_load_bitmap("Images/console_keyboard.png");             // Carregando o bit map do telclado
	al_convert_mask_to_alpha(b_console_keyboard, al_map_rgb(255, 0, 255));          // Tornando-lhe o fundo transparente
	b_console_keyboard_left = al_load_bitmap("Images/console_keyboard_left.png");   // Carregando o outro bit map do teclado
	al_convert_mask_to_alpha(b_console_keyboard_left, al_map_rgb(255, 0, 255));     // Tornando-lhe o fundo transparente
	b_console_keyboard_right = al_load_bitmap("Images/console_keyboard_right.png"); // Carregando o ultimo bit map do teclado
	al_convert_mask_to_alpha(b_console_keyboard_right, al_map_rgb(255, 0, 255));    // Tornando-lhe o fundo transparente

    // Incializando o console
    c_initialize(&console, WIDTH, HEIGHT, b_console, b_console_keyboard, b_console_keyboard_left, b_console_keyboard_right);

	event_queue = al_create_event_queue(); // Criando lista de evento
	timer = al_create_timer(1.0 / FPS);    // Criando timer

	al_register_event_source(event_queue, al_get_keyboard_event_source());       // Registrar eventos do teclado
	al_register_event_source(event_queue, al_get_timer_event_source(timer));     // Registrar eventos do timer
	al_register_event_source(event_queue, al_get_display_event_source(display)); // Registrar eventos do display

	al_start_timer(timer);  // Iniciando o timer
    srand(time(NULL));      // Gerando semente para os números aleatórios

    // JOGO
    while(!end){
        ALLEGRO_EVENT ev;                    // Criando variável que armazenará o evento
		al_wait_for_event(event_queue, &ev); // Esperando por um evento

		// Verificando se o evento foi do tipo tempo
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			draw = true;   // Indica que é para desenhar os objetos na tela
			update = true; // Indica que se deve atualizar toda lógica do jogo
		}
		// Verifica se o evento foi fechar janela do jogo
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			end = true; // Termina o jogo
		}
		// Verifica se o evento foi pressionar alguma tecla para baixo
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
                case ALLEGRO_KEY_ESCAPE:
                    end = true;          // Caso seja o esc, então o jogo termina
                    break;
                case ALLEGRO_KEY_H:
                    debug = !debug;      // Caso seja o H, então ativa o modo debug
                    break;
                default:
                    break;
			}
		}

        // Atualiza a lógica do jogo
		if(update){

            s_forward(&snake);               // Move a cobra para frente
            f_update(&field, apple, snake);  // Atualiza o campo
            a_generate(&apple, field);       // Gera uma maçã aleatoriamente
            s_eat(&snake, &apple);           // Verifica se a cobra comeu a maçã no campo

            // Verifica se a cobra colidiu consigo mesmo
            if(s_collision(snake)){
                end = true;  // Caso sim, termina o jogo
            }
            // Obtém o estado de cada tecla
            al_get_keyboard_state(&keyState);

            // Verifica se a tecla left está sendo pressionada
            if(al_key_down(&keyState, ALLEGRO_KEY_LEFT)){
                s_rotation_to_right(&snake);          // Modifica a direção do vetor velocidade da cobra
                console.state_keyboard = PRESS_LEFT;  // Atualiza a imagem do console
            }
            // Verifica se a tecla right está sendo pressionada
            else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT)){
                s_rotation_to_left(&snake);           // Modifica a direção do vetor velocidade da cobra
                console.state_keyboard = PRESS_RIGHT; // Atualiza a imagem do console
            }
            else{
                console.state_keyboard = NOTHING;     // Atualiza a imagem do console
            }
            update = false;
		}

		// Verifica se pode desenhar na tela
        if(draw && al_is_event_queue_empty(event_queue))
		{
            al_clear_to_color(al_map_rgb(0,0,0));                   // Limpa a tela
            f_draw(field, (int)(snake.size)/(GROWTH_SPEED), font2); // Desenha o campo
            a_draw(apple);                                          // Desenha a maçã
            s_draw(snake);                                          // Desenha a cobra
            c_draw(console);                                        // Desenha o console
            if(debug){
                g_debug(field, snake, apple, font1); // Caso o modo debug esteja ativado, desenha na tela algumas informações adicionais
            }
            // Atualiza a tela do jogo
		    al_flip_display();

			draw = false;
		}

    }

    // Destruindo todas as variáveis que foram criadas (BIT MAPS, DISPLAY, LISTA DE EVENTOS...)
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_font(font1);
    al_destroy_font(font2);
    al_destroy_bitmap(b_snake_head);
	al_destroy_bitmap(b_snake_body);
	al_destroy_bitmap(b_snake_tail);
	al_destroy_bitmap(b_field);
	al_destroy_bitmap(b_apple);
	al_destroy_bitmap(b_console);
	al_destroy_bitmap(b_console_keyboard);
    al_destroy_bitmap(b_console_keyboard_left);
    al_destroy_bitmap(b_console_keyboard_right);
	al_destroy_timer(timer);

	return 0;
}
