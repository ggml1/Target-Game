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
ALLEGRO_BITMAP *Shadow;
ALLEGRO_BITMAP *TileC;
ALLEGRO_BITMAP *red;
ALLEGRO_BITMAP *blue;
ALLEGRO_BITMAP *heart_14;
ALLEGRO_BITMAP *heart_24;
ALLEGRO_BITMAP *heart_34;
ALLEGRO_BITMAP *heart_44;
ALLEGRO_BITMAP *spaceTecla;

typedef struct{
	short int esq_dir;
	short int flag;
	bool situacao;
	short int teamPos;
	short int tipoPacote;				// 0 -> msg inicial (nickname), 1 - > movimento
    char playerName[16];
    short int HP;
    char mov;
    short int playerX;
    short int playerY;
    bool saiu;
}Player;

typedef struct{
	short int posicao[6][3];
	char nicks[6][10];
	short int qtdPlayers;
	short int situacao[6];
	short int aux;
	bool comecaJogo;
}Lobby;

typedef struct{
	//int tipoPacote; nao precisa, pois o server mandara pro client e o client nao precisa ter um switch case de opcoes de moves.
	short int tag;
	short int HP;
	short int oldx;
	short int oldy;
	short int newx;
	short int newy;
	short int idMoved;
	char olhando[9];
}Moves;

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
