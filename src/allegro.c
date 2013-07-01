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
#include <allegro5/allegro_image.h>

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

static void gui_keyboard (int move);

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
    al_init_image_addon();

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
    boat = NULL;
    heart = NULL;

    /* Criamos a nossa janela - dimensões de largura x altura pixels */
    window = al_create_display(length, height);
    boat = al_load_bitmap("b3.png");
    heart = al_load_bitmap("heart.png");
    boat_height = al_get_bitmap_height(boat);
    boat_width = al_get_bitmap_width(boat);

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

        /* 1º Caso: fechar a janela */
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_event_queue(event_queue);
            gui_window_destroy();
            return CLOSE;
        }
        /* 2º Caso: pressionando tecla */
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LEFT:
                    gui_keyboard(MOVE_LEFT);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    gui_keyboard(MOVE_RIGHT);
                    break;
            }
        }
        /* 3º caso: soltando tecla */
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(event.keyboard.keycode)
            {
                case ALLEGRO_KEY_LEFT:
                    gui_keyboard(STOP_LEFT);
                    break;
                case ALLEGRO_KEY_RIGHT:
                    gui_keyboard(STOP_RIGHT);
                    break;
            }
        }
    } /* while */
    return 0;
}

void gui_window_destroy()
{
    if(window != NULL)
        al_destroy_display(window);
    window = NULL;
}

void gui_window_clear  (void)    { al_clear_to_color(al_map_rgb(0, 0, 0)); }

void gui_window_update (int lifes)
{
    gui_river_heart(lifes);
    al_flip_display();
}

void gui_window_delay  (float t) { al_rest(t); }

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


void gui_river_water(float x1, float y1, float x2, float y2)
{
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(135, 206, 250));
}

void gui_river_land(float x1, float y1, float x2, float y2)
{
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(139, 69, 19));
}

void gui_river_smooth_water(float x1, float y1, float x2, float y2, float x3, float y3)
{
    al_draw_filled_triangle(x1, y1 ,x2, y2, x3, y3, al_map_rgb(135, 206, 250));
}

void gui_river_smooth_land(float x1, float y1, float x2, float y2, float x3, float y3)
{
    al_draw_filled_triangle(x1, y1 ,x2, y2, x3, y3, al_map_rgb(139, 69, 19));
}

void gui_river_heart(int lifes)
{
    extern int frame_length;
    int i;
    for(i = lifes; i > 0; i--)
        al_draw_rotated_bitmap(heart, 0, 0, frame_length-20*i, 0, 0,0);
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BARCO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

void gui_boat_draw(int *x, int *y, int prop)
{
    /* Realiza o movimento */
    int rotate;
    if(mov[LEFT])
    {
       (*x) -= 0.5 * prop;
       rotate = -1;
    }
    else if(mov[RIGHT])
    {
        (*x) += 0.5 * prop;
        rotate = 1;
    }
    else rotate = 0;
    if(mov[DOWN])  (*y) -= 0.5 * prop;
    if(mov[UP])    (*y) += 0.5 * prop;

    al_draw_rotated_bitmap(boat, boat_width/2, boat_height/2, (*x * prop), (*y * prop)-30.0, rotate*ALLEGRO_PI/8, 0);
    /*al_draw_bitmap(boat, (*x*prop), (*y*prop)-60, 0);*/
    /*al_draw_filled_ellipse((*x * prop), (*y * prop)-20.0,
            10.0, 20.0, al_map_rgb(139, 87, 66));*/
      /*rotate_sprite(xcf, "canoa.xcf",25, 5, 45); */

}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                             MOVIMENTO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
static void gui_keyboard(int key)
{
    switch(key)
    {
        /* Caso padrão para apenas animar */
        case NOP:
            break;

        /* Inicia os movimentos: */
        case MOVE_LEFT:
            mov[LEFT] = 1;
            move--;
            break;

        case MOVE_RIGHT:
            mov[RIGHT] = 1;
            move ++;
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
