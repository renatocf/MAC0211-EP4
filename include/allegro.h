#ifndef H_GUI_DEFINED
#define H_GUI_DEFINED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro.h>
#include "river.h"


/* Variáveis globais para criação de janelas e eventos */
ALLEGRO_DISPLAY *window;
ALLEGRO_EVENT_QUEUE *event_queue;

/* Inicialização da GUI */
void gui_init();

/* Funções de criação de janelas */
void gui_window_clear   (void);
void gui_window_update  (void);
void gui_window_create  (int length, int height);
void gui_window_destroy (void);

/* Funções de manipulação do rio */
void gui_river_create_land   (float x1, float y1);
void gui_river_create_water  (float x1, float y1);
void gui_river_create_margin (int x1, int y1,
                              int x2, int y2,
                              int x3, int y3);

/* Funções para manipulação do barco */
void gui_boat_draw   (void);
void gui_boat_move   (int move);
void gui_boat_start  (float x, float y);
void gui_boat_shock  (int n);

/* Funções para o teclado */
int  gui_keyboard_init (void);

#define CLOSE 1
int gui_event_get(void);

#endif
