#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60

double startingTime;
double startingTimeBlink;


ALLEGRO_DISPLAY *main_window;
ALLEGRO_EVENT_QUEUE *eventsQueue;


ALLEGRO_FONT *font_1;
ALLEGRO_FONT *font_2;

ALLEGRO_BITMAP *bit1;
ALLEGRO_BITMAP *bit2;
ALLEGRO_BITMAP *bit3;
ALLEGRO_BITMAP *bit4;
ALLEGRO_BITMAP *tileSet;
ALLEGRO_BITMAP *menuImg;
ALLEGRO_BITMAP *seta;

typedef struct{
	int teamPos;
	int tipoPacote;				// 0 -> msg inicial (nickname), 1 - > movimento
    char playerName[16];
    char mov;                   // u -> up, d -> down, l -> left, r -> right, s -> space
    int playerX;
    int playerY;
}Player;

///1 2 3 -> time vermelho
// 1 -> [22][1];
// 2 -> [21][1];
// 3 -> [20][1];
// 4 5 6 -> time azul
// 4 -> [1][30];
// 5 -> [1][29];
// 6 -> [1][28];


//Funções Centrais da Allegro
bool coreInit();
bool windowInit(int W, int H, char title[]);
bool inputInit();
bool fontInit();
void allegroEnd();

//Funções para Controle da quantidade de Frames por Segundo
void startTimer();
bool Blink();
void startTimerBlink();
double getTimer();
double getTimerBlink();
void FPSLimit();

//Função de Carregamento de Gráficos
bool loadGraphics();

#endif
