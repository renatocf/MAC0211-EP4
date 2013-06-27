/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
/* Bibliotecas-padrão */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Bibliotecas gráficas */
#include "allegro.h"

/* bibliotecas internas */
#include "list.h"
#include "utils.h"
#include "strip.h"
#include "river-internal.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                JOGADOR
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

typedef struct player Player;
struct player {
    int lifes;
};

Player P1;

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                         FUNÇÕES DE CONFIGURAÇÃO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
void river_config_flux (float flux)
{
    Config.flux = flux;
}

void river_config_island (float prob_island, int freq_island)
{
    Config.prob_island = prob_island;
    Config.freq_island = freq_island;
}

void river_config_size (int length, int height)
{
    Config.height = height;
    Config.length = length;
}

void river_config_margins (int zone)
{
    Config.zone = zone;
}

static int frame_height;
int old_left_margin = -1; int old_right_margin = -1;
TStrip strip1;

int boat_vpos;
int boat_hpos;


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          FUNÇÕES DE ANIMAÇÃO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
void river_animation_init()
{
    gui_init();
    boat_hpos = (int) (Config.length/2.0); 
    boat_vpos = frame_height;
    
    gui_window_create(Config.length * 5, Config.height * 5);
    
    /* Inicializa informações do Jogador */
    P1.lifes = 10;
}

void river_animation_generate(int seed)
{
    /** VARIÁVEIS *****************************************************/
        int i = 0;                       /* Contador                 */
        TStrip new_line;                 /* Linha gerada             */
        TStrip first_line;               /* 1ª linha gedara          */
        int zone = Config.zone;          /* 'Zona de conforto'       */
        float flux = Config.flux;        /* Fluxo total do rio       */
        int height = Config.height;      /* Altura total do grid     */
        int length = Config.length;      /* Largura total do rio     */
        int freq = Config.freq_island;   /* Distância entre ilhas    */
        float prob = Config.prob_island; /* Probabilidade de ilhas   */
        Link new_node;
        
        frame_height = 0; /* Zera altura do frame de impressão do rio */
        
        /* Inicializa semente geradora de faixas de terreno
         * e cria lista para colocá-las: nosso cenário */
        tstrip_seed(seed);
        tstrip_island(prob, freq);
        river = list_init(height);
        
    /** INICIALIZA RIO ************************************************/
        /* Primeira linha, que servirá de base para todo o rio */
        first_line = tstrip_generate(length, zone, flux, NO_BASE, NULL);
        new_node = list_new_node(first_line);
        
        /* Preenche 'altura' faixas de terreno na lista: */
        list_insert(river, new_node);
        
        for(i = 1, base = first_line; i < height; i++, base = new_line)
        {
            new_line = tstrip_generate(length, zone, flux, base, NULL);
            new_node = list_new_node(new_line);
            list_insert(river, new_node);
        }
        
    /** IMPRIME RIO ***************************************************/
        gui_window_clear();
        /* gui_boat_start((Config.length/2.0) * 5, frame_height); */
        list_select(river, HEAD, strip_print);
        gui_window_update();
}

int river_animation_iterate()
{
    /** VARIÁVEIS *****************************************************/
        float flux = Config.flux;       /* Fluxo do rio              */
        int length = Config.length;     /* Largura do grid           */
        int zone = Config.zone;         /* Zona de conforto          */
        Link new_node;                  /* Nódulo novo da lista      */
        TStrip top;                     /* Faixa superior do rio     */

        frame_height = 0; /* Zera altura do frame que imprime o rio */
        
    /** AVANÇA FAIXA DE TERRENO ***************************************/
        /* Libera linha do topo do grid ('saindo da tela') */
        new_node = list_prev(list_head(river));
        list_remove(river, new_node);
        
        /* Cria linha da base do grid ('entrando na tela') */
        top = tstrip_generate(length, zone, flux, base, list_item(new_node));
        base = top; list_insert(river, new_node);
        
    /** IMPRIME RIO ***************************************************/
        if(gui_event_get() == CLOSE) return EXIT_FAILURE;
        gui_window_clear();
        list_select(river, HEAD, strip_print);
        
        printf("%d %d\n", boat_hpos, frame_height);
        gui_boat_draw(&boat_hpos, &boat_vpos, 5);
        
        /* Barco bateu, recomeça do meio */
        if(strip1[boat_hpos].t == LAND 
        || strip1[boat_hpos-1].t == LAND
        || strip1[boat_hpos-2].t == LAND 
        || strip1[boat_hpos+1].t == LAND)
        {
            P1.lifes--; gui_boat_shock(P1.lifes);
            boat_hpos = (int) (Config.length/2.0);
            boat_vpos = frame_height/5;
        }
        
        gui_window_update();
        
        /* Fim de Jogo */
        if(P1.lifes == 0) return GAME_OVER;
        return EXIT_SUCCESS;
}

void strip_print(TStrip strip)
{
    /** VARIÁVEIS *****************************************************/
        int old_m_l = 0;    /* Posição da margem esquerda anterior   */
        int old_m_r = 0;    /* Posição da margem direita anterior    */
        int i = 0, k = 0;   /* Contadores auxiliares                 */
        int control = 0;    /* Variável auxiliar                     */
        strip1 = strip;
        
    /** ANÁLISE DO RIO ************************************************/
        for(i = 0; i < Config.length-1; i++)
            if(strip[i].t == WATER && strip[i+1].t == LAND) k++;

        for(i = 0; i < Config.length; i++)
        {
            /* Começo de rio -- sem um elemento anterior */
            if(old_left_margin== -1)
                gui_river_create_land(i*5, frame_height);
            if(i==Config.length-1 && old_right_margin == -1)
                gui_river_create_land(i*5, frame_height);

            /* Terra seguida de água */
            else if(i!= Config.length-1
                    && strip[i].t == LAND
                    && strip[i+1].t == WATER
                    && (++control) == 1)
            {
                old_m_l = i;
                if(i > old_left_margin)
                    gui_river_create_margin(i*5, frame_height+5, 
                        (i+1)*5, frame_height+5, i*5, frame_height);
                else
                    gui_river_create_land(i*5, frame_height);
            }

            /* Terra seguida de água - pega a água depois de terra */
            else if(i!=0
                    && strip[i-1].t == LAND
                    && strip[i].t == WATER
                    && control == 1)
            {
                if(i-1 < old_left_margin)
                    gui_river_create_margin(i*5, frame_height, 
                        (i+1)*5, frame_height, i*5, frame_height+5);
                else
                    gui_river_create_water(i*5, frame_height);
            }

            /* Água seguida de terra */
            else if(i!=0
                    && strip[i].t == LAND
                    && strip[i-1].t == WATER)
            {
                old_m_r = i;
                if(i < old_right_margin 
                && ((k>=2 && control == 2) || (k<2 && control == 1)))
                    gui_river_create_margin((i+1)*5, frame_height+5, 
                        i*5, frame_height+5, (i+1)*5, frame_height);
                else
                    gui_river_create_land(i*5, frame_height);
            }

            /* Água seguida de terra - analisa água antes da terra */
            else if(i != Config.length-1
                    && strip[i].t == WATER
                    && strip[i+1].t == LAND)
            {
                if(i+1 > old_right_margin
                && ( (k>=2 && control == 2) || (k<2 && control == 1) ))
                    gui_river_create_margin((i+1)*5, frame_height, 
                        i*5, frame_height, (i+1)*5, frame_height+5);
                else
                    gui_river_create_water(i*5, frame_height);
            }
            else if(strip[i].t == LAND)
                gui_river_create_land(i * 5, frame_height);
            else if(strip[i].t == WATER)
                gui_river_create_water(i * 5, frame_height);

        }
        
        al_rest(1.4e-5);
        old_left_margin = old_m_l;
        old_right_margin = old_m_r;
        
        frame_height += 5;
}

void river_animation_finish()
{
    list_free(river);
    river = NULL; base = NULL;
    gui_window_destroy();
}
