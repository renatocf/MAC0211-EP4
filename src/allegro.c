/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Bibliotecas externas */
#include <stdio.h>
#include <stdlib.h>

/* Bibliotecas gráficas */
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

/* Bibliotecas internas */
#include "allegro.h"

/* Macros */
#define LEFT  0
#define DOWN  1
#define UP    2
#define RIGHT 3

#define NOP         0
#define MOVE_LEFT   1
#define STOP_LEFT  -1
#define MOVE_RIGHT  2
#define STOP_RIGHT -2

#define MOVE_UP     3
#define STOP_UP    -3
#define MOVE_DOWN   4
#define STOP_DOWN  -4


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                       PROTÓTIPOS INTERNOS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

static void gui_boat_move (int move);

/* Esquerda, baixo, cima, direita */
static int mov[4] = {0, 0, 0, 0}; 

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          INICIALIZAÇÃO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void gui_init()
{
    /* Inicializa a biblioteca allegro */
    al_init();
    al_init_primitives_addon();
    
    /* Cria fila de eventos */
    event_queue = NULL;
    event_queue = al_create_event_queue();
    
    if(!gui_keyboard_init())
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              JANELA
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void gui_window_create(int length, int height)
{
    /* Variável representando a janela principal */
    window = NULL;
    
    /* Criamos a nossa janela - dimensões de largura x altura pixels */
    window = al_create_display(length, height);

    /* Preenchemos a janela de branco */
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    /* Atualiza a tela */
    al_flip_display();
    if (!event_queue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        gui_window_destroy();
    }
    
    al_set_window_title(window, "Jogo da canoa");
    
    /* Associa teclado com a janela */
    al_register_event_source(event_queue, 
        al_get_display_event_source(window));
}

int gui_event_get(void)
{
    if (!al_is_event_queue_empty(event_queue))
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        { 
            al_destroy_event_queue(event_queue);
            gui_window_destroy();
            return CLOSE; 
        }
        
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LEFT:
                    gui_boat_move(1);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    gui_boat_move(2);
                    break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LEFT:
                    gui_boat_move(-1);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    gui_boat_move(-2);
                    break;
            }
        }
    }
    return 0;
}

void gui_window_destroy()
{
    if(window != NULL) 
        al_destroy_display(window); 
    window = NULL;
}

void gui_window_clear()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void gui_window_update()
{
    al_flip_display();
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                               TECLADO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

int gui_keyboard_init(void)
{ 
    /* Inicializa teclado */
    if(!al_install_keyboard()) return 0; /* ERRO */
    
    /* Associa teclado à fila de eventos */
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    return 1; /* SUCESSO */
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                RIO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void gui_river_create_land(float x1, float y1)
{
    al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(139, 69, 19));
}

void gui_river_create_water(float x1, float y1)
{
    al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(100, 149, 237));
}

void gui_river_create_margin(int x1, int y1, int x2, int y2, int x3, int y3)
{
    /* Triângulo preenchido: x1, y1, x2, y2, x3, y3, cor*/
    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(139, 69, 19));
    al_draw_filled_triangle(x2, y2, x3, y3, x2, y3, al_map_rgb(100, 149, 237));
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BARCO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
/*
static int boat_vpos, boat_hpos;

void gui_boat_start(float x, float y)
{
    boat_vpos = x; boat_hpos = y;
}
*/
void gui_boat_draw(int *x, int *y, int prop)
{
    /* Realiza o movimento */
    if(mov[LEFT])  (*x) -= 0.5 * prop;
    if(mov[RIGHT]) (*x) += 0.5 * prop;
    if(mov[DOWN])  (*y) -= 0.5 * prop;
    if(mov[UP])    (*y) += 0.5 * prop;
    
    al_draw_filled_ellipse((*x * prop), (*y * prop)-20.0, 
            10.0, 20.0, al_map_rgb(139, 87, 66));
}

void gui_boat_shock(int n)
{
    /* Mensagens da batida */
    const char title[]   = "Jogo das Canoas";
    const char heading[] = "Atenção:";
    const char text[]    = "Você perdeu uma vida!! ";
    
    /* Cria e exibe diálogo de erro */
    al_show_native_message_box(
        NULL, title, heading, text, NULL, ALLEGRO_MESSAGEBOX_WARN);
    
    /* Reseta movimentos */
    mov[0] = 0; mov[1] = 0; mov[2] = 0; mov[3] = 0;
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                             MOVIMENTO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
static void gui_boat_move(int key)
{
    switch(key)
    {
        /* Caso padrão para apenas animar */
        case NOP:
            break;

        /* Inicia os movimentos: */
        case MOVE_LEFT:
            mov[LEFT] = 1;
            break;
        
        case MOVE_RIGHT:
            mov[RIGHT] = 1;
            break;
        
        /* Interrompe os movimentos: */
        case STOP_LEFT:
            mov[LEFT] = 0;
            break;
        
        case STOP_RIGHT:
            mov[RIGHT] = 0;
            break;

        /* Caso padrão: não faça nada */
        default:
            break;
    }
}
