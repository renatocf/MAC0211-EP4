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

/* Bibliotecas internas */
#include "allegro.h"

/* Macros */
#define WINDOW_DEFATULT_COLOR (0, 0, 0)

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
    al_init(); al_init_primitives_addon();
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
    event_queue = NULL;
    event_queue = al_create_event_queue();
    
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
        al_destroy_display(window);
    }

    al_set_window_title(window, "Jogo da canoa");
    al_register_event_source(event_queue, al_get_display_event_source(window));
}

int gui_window_destroy(void)
{
    if (!al_is_event_queue_empty(event_queue))
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_destroy_event_queue(event_queue);
            al_destroy_display(window);
            return 1;
        }
    }
    return 0;
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

void gui_boat_create(float x, float y)
{
    al_draw_filled_ellipse(x, y -20.0 ,10.0 , 20.0, al_map_rgb(139, 87, 66));
}

