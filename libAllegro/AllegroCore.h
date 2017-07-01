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
ALLEGRO_FONT *font_3;

ALLEGRO_BITMAP *bit1;
ALLEGRO_BITMAP *bit2;
ALLEGRO_BITMAP *bit3;
ALLEGRO_BITMAP *bit4;
ALLEGRO_BITMAP *tileSet;
ALLEGRO_BITMAP *menuInicio;
ALLEGRO_BITMAP *menuOpcoes;
ALLEGRO_BITMAP *menuNormal;
ALLEGRO_BITMAP *seta;
ALLEGRO_BITMAP *upKey;
ALLEGRO_BITMAP *downKey;
ALLEGRO_BITMAP *leftKey;
ALLEGRO_BITMAP *rightKey;
ALLEGRO_BITMAP *Dungeon_B;
ALLEGRO_BITMAP *Outside_B;
ALLEGRO_BITMAP *World_B;
ALLEGRO_BITMAP *Dungeon_A2;
ALLEGRO_BITMAP *Inside_C;

typedef struct{
	int teamPos;
	int tipoPacote;				// 0 -> msg inicial (nickname), 1 - > movimento
    char playerName[16];
    int HP;
    char mov;
    int playerX;
    int playerY;
    int mapa[24][32];
}Player;

typedef struct{
	//int tipoPacote; nao precisa, pois o server mandara pro client e o client nao precisa ter um switch case de opcoes de moves.
	int oldx;
	int oldy;
	int newx;
	int newy;
	int idMoved;
	char olhando[9];
}Moves;

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
