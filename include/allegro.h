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
ALLEGRO_BITMAP *boat;
ALLEGRO_BITMAP *heart;

/* Acumula o número de vezes seguidas que se pressiona uma tecla */
int move;
float boat_angle;
float speedy;
float boat_height;
float boat_width;

/* Inicialização da GUI */
void gui_init();

/* Funções de criação de janelas */
void gui_window_clear   (void);
void gui_window_delay   (float t);
void gui_window_update  (int lifes);
void gui_window_create  (int length, int height);
void gui_window_destroy (void);

/* Funções de manipulação do rio */
void gui_river_water(float x1, float y1, float x2, float y2);
void gui_river_land(float x1, float y1, float x2, float y2);
void gui_river_smooth_water(float x1, float y1, float x2, float y2, float x3, float y3);
void gui_river_smooth_land(float x1, float y1, float x2, float y2, float x3, float y3);
void gui_river_heart(int lifes);


/* Funções para manipulação do barco */
void gui_boat_draw   (int *x, int *y, int proportion);
void gui_boat_start  (float x, float y);
void gui_boat_shock  (int n);

/* Funções para o teclado */
int  gui_keyboard_init (void);

#define CLOSE 1
int gui_event_get(void);

#endif
