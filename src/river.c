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
#define pi 3.14159

TStrip strip1;
/*float boat_angle = 0;*/
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


int old_right_margin = -1;
int old_left_margin = -1;

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
    boat_vpos = frame_height/5;

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

    frame_length = length*5;
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
    list_select(river, HEAD, strip_print);

    boat_hpos = (int) (Config.length/2.0);
    boat_vpos = frame_height/5;
    gui_boat_draw(&boat_hpos, &boat_vpos, 5);


    gui_window_update(P1.lifes);
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
    base = top;
    list_insert(river, new_node);

    /** IMPRIME RIO ***************************************************/
    if(gui_event_get() == CLOSE) return EXIT_FAILURE;
    gui_window_clear();
    list_select(river, HEAD, strip_print);

    /*printf("%d %d\n", boat_hpos, boat_vpos);*/
    gui_boat_draw(&boat_hpos, &boat_vpos, 5);
    /* Velocidade */
    speedy = 0.6 * strip1[boat_hpos].v + (sqrt(cos(boat_angle)*cos(boat_angle))+1) * speedy*0.4;
    printf("_________%f                      %f   %f\n ", speedy, strip1[boat_hpos].v, sqrt(cos(boat_angle)*cos(boat_angle)));

    /* Barco bateu, recomeça do meio */
    if(strip1[boat_hpos].t == LAND
            || strip1[boat_hpos-1].t == LAND
            || strip1[boat_hpos-2].t == LAND
            || strip1[boat_hpos+1].t == LAND/*
            || strip1[boat_hpos+2].t == LAND*/)
        /*if(base[boat_hpos].t == LAND
        || base[boat_hpos-1].t == LAND
        || base[boat_hpos-2].t == LAND
        || base[boat_hpos+1].t == LAND)*/
    {
        P1.lifes--; /*gui_boat_shock(P1.lifes);*/
        move = 0;
        boat_hpos = (int) (Config.length/2.0);
        boat_vpos = frame_height/5;
    }
    else
    {
        /*boat_angle += ( move_right - move_left ) * ALFA + pi / 2;
        boat_speed = strip1[boat_hpos].v + sin(boat_angle)*(strip1[boat_hpos-1].v + strip1[boat_hpos+1].v)/2.0;*/

        boat_angle = (pi/2.0)*(1- (10.0 * move/Config.length));
    }

    gui_window_update(P1.lifes);

    /* Fim de Jogo */
    if(P1.lifes == 0) return GAME_OVER;
    return EXIT_SUCCESS;
}

void strip_print(TStrip strip)
{
    /** VARIÁVEIS *****************************************************/
        int i;                        /* Contador auxiliar          */
        int new_left_margin;          /* Posição nova margem esq.   */
        int new_right_margin;         /* Posição nova margem dir.   */
        int island_left_margin = -1;  /* Posição margem esq. ilha   */
        int island_right_margin = -1; /* Posição margem dir. ilha   */
        strip1 = strip;

    /** ENCONTRA MARGEM ESQUERDA **************************************/
        for(i = 0; i < Config.length; i++)
            if(strip[i].t == LAND && strip[i+1].t == WATER)
            {
                new_left_margin = i;
                break;
            }

    /** ENCONTRA MARGEM DIREITA ***************************************/
        for(i = Config.length-1; i > 0; i--)
            if(strip[i].t == LAND && strip[i-1].t == WATER)
            {
                new_right_margin = i;
                break;
            }

    /** ENCONTRA ILHA *************************************************/
        for(i = new_left_margin + 1; i < new_right_margin; i++)
        {
            /* Margem direita */
            if(strip[i].t == WATER && strip[i+1].t == LAND)
                { island_left_margin = i+1; }

            /* Margem esquerda */
            if(strip[i].t == LAND && strip[i+1].t == WATER)
                { island_right_margin = i; break; }
        }

    /** 1º CASO: ÁGUA *************************************************/
        /* 1.1: Não há ilhas - imprime água entre as margens */
        if(island_left_margin == -1 && island_right_margin == -1)
        {
            gui_river_water(
                new_left_margin*5,          /* x1 */
                frame_height,               /* y1 */
                new_right_margin*5,         /* x2 */
                frame_height+5              /* y2 */
            );
        }
        /* 1.2: Exitem ilhas - imprime água no entorno da ilhas */
        else
        {
            gui_river_water( /* Esquerda */
                new_left_margin*5,          /* x1 */
                frame_height,               /* y1 */
                island_left_margin*5,       /* x2 */
                frame_height+5              /* y2 */
            );

            gui_river_water( /* Direita */
                (island_right_margin+1)*5,  /* x1 */
                frame_height,               /* y1 */
                new_right_margin*5,         /* x2 */
                frame_height+5              /* y2 */
            );
        }

    /** 2º CASO: TERRA ************************************************/
        /* 2.1: Há margens maiores que 1 quadrado */
        if(old_right_margin != -1 && old_left_margin != -1)
        {
            /*
             *  2.1.1: MARGEM ESQUERDA
             */

            /* 2.1.1.1: Margem esquerda à frente da nova margem */
            if(old_left_margin > new_left_margin)
            {
                /* Terra esquerda */
                gui_river_land(
                    0,                      /* x1 */
                    frame_height,           /* y1 */
                    old_left_margin*5,      /* x2 */
                    frame_height+5          /* y2 */
                );

                /* Triângulo para a terra */
                gui_river_smooth_land(
                    old_left_margin*5,      /* x1 */
                    frame_height,           /* y1 */
                    old_left_margin*5,      /* x2 */
                    frame_height+5,         /* y2 */
                    (old_left_margin+1)*5,  /* x3 */
                    frame_height            /* y3 */
                );

                /* Triângulo para a água */
                gui_river_smooth_water(
                    old_left_margin*5,      /* x1 */
                    frame_height+5,         /* y1 */
                    (old_left_margin+1)*5,  /* x2 */
                    frame_height+5,         /* y2 */
                    (old_left_margin+1)*5,  /* x3 */
                    frame_height            /* y3 */
                );
            }

            /* 2.1.1.2: Margem esquerda anterior à nova margem */
            else if(old_left_margin < new_left_margin)
            {
                /* Terra esquerda */
                gui_river_land(
                    0,                      /* x1 */
                    frame_height,           /* y1 */
                    new_left_margin*5,      /* x2 */
                    frame_height+5          /* y2 */
                );

                /* Triângulo para a terra */
                gui_river_smooth_land(
                    new_left_margin*5,      /* x1 */
                    frame_height,           /* y1 */
                    new_left_margin*5,      /* x2 */
                    frame_height+5,         /* y2 */
                    (new_left_margin+1)*5,  /* x3 */
                    frame_height+5          /* y3 */
                );

                /* Triângulo para a água */
                gui_river_smooth_water(
                    new_left_margin*5,      /* x1 */
                    frame_height,           /* y1 */
                    (new_left_margin+1)*5,  /* x2 */
                    frame_height,           /* y2 */
                    (new_left_margin+1)*5,  /* x3 */
                    frame_height+5          /* y3 */
                );
            }

            /* 2.1.1.3: Margem esquerda e nova margem retas */
            else
            {
                /* Terra esquerda */
                gui_river_land(
                    0,                      /* x1 */
                    frame_height,           /* y1 */
                    (new_left_margin+1)*5,  /* x2 */
                    frame_height+5          /* y2 */
                );
            }

            /*
             *  2.1.2: MARGEM DIREITA
             */

            /* 2.1.2.1: Margem direita à frente da nova direita */
            if(old_right_margin < new_right_margin)
            {
                /* Terra direita */
                gui_river_land(
                    new_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    frame_length,           /* x2 */
                    frame_height+5          /* y2 */
                );

                /* Triângulo para a terra */
                gui_river_smooth_land(
                    old_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    new_right_margin*5,     /* x2 */
                    frame_height,           /* y2 */
                    new_right_margin*5,     /* x3 */
                    frame_height+5          /* y3 */
                );

                /* Triângulo para a água */
                gui_river_smooth_water(
                    old_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    old_right_margin*5,     /* x2 */
                    frame_height+5,         /* y2 */
                    new_right_margin*5,     /* x3 */
                    frame_height+5          /* y3 */
                );
            }

            /* 2.1.2.2: Margem direita anterios à nova direita */
            else if(old_right_margin > new_right_margin)
            {
                /* Terra direita */
                gui_river_land(
                    old_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    frame_length,           /* x2 */
                    frame_height+5          /* y2 */
                );

                /* Triângulo para a terra */
                gui_river_smooth_land(
                    new_right_margin*5,     /* x1 */
                    frame_height+5,         /* y1 */
                    old_right_margin*5,     /* x2 */
                    frame_height+5,         /* y2 */
                    old_right_margin*5,     /* x3 */
                    frame_height            /* y3 */
                );

                /* Triângulo para a água */
                gui_river_smooth_water(
                    new_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    new_right_margin*5,     /* x2 */
                    frame_height+5,         /* y2 */
                    old_right_margin*5,     /* x3 */
                    frame_height            /* y3 */
                );
            }

            /* 2.1.2.3: Margem direita e nova margem retas */
            else
            {
                /* Terra direita */
                gui_river_land(
                    new_right_margin*5,     /* x1 */
                    frame_height,           /* y1 */
                    frame_length,           /* x2 */
                    frame_height+5          /* y2 */
                );
            }

            /*
             *  2.1.3: ILHAS
             */

            if(island_left_margin != -1 && island_right_margin != -1)
                /* Coloca terra no intervalo das ilhas */
                gui_river_land(
                    island_left_margin*5,       /* x1 */
                    frame_height,               /* y1 */
                    (island_right_margin+1)*5,  /* x2 */
                    frame_height+5              /* y2 */
                );
        }

        /* 2.2: Margens com apenas 1 quadrado */
        else
        {
            gui_river_land(
                0,                      /* x1 */
                frame_height,           /* y1 */
                new_left_margin*5,      /* x2 */
                frame_height+5          /* y2 */
            );

            gui_river_land(
                new_right_margin*5,     /* x1 */
                frame_height,           /* y1 */
                frame_length,           /* x2 */
                frame_height+5          /* y2 */
            );
        }

    /** CONFIGURAÇÕES PARA NOVA ITERAÇÃO ******************************/
        /* Atraso da função para futura exibição do frame */
        gui_window_delay(0.22* pow(10, -(3.0 + speedy*2.0)));

        /* Salva margens esquerda/direita recém
         * criadas para a nova iteração da impressão */
        old_left_margin = new_left_margin;
        old_right_margin = new_right_margin;

        /* Somando à altura atual do frame a constante de
         * proporção da exibição do rio */
        frame_height += 5;
}

void river_animation_finish()
{
    list_free(river);
    river = NULL;
    base = NULL;
    gui_window_destroy();
}
