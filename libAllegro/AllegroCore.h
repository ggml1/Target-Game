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
ALLEGRO_BITMAP *telaMorte;
ALLEGRO_BITMAP *bmagicr;
ALLEGRO_BITMAP *bmagicl;
ALLEGRO_BITMAP *bmagicu;
ALLEGRO_BITMAP *bmagicd;
ALLEGRO_BITMAP *rmagicr;
ALLEGRO_BITMAP *rmagicl;
ALLEGRO_BITMAP *rmagicd;
ALLEGRO_BITMAP *rmagicu;
ALLEGRO_BITMAP *barrowu;
ALLEGRO_BITMAP *barrowl;
ALLEGRO_BITMAP *barrowd;
ALLEGRO_BITMAP *barrowr;
ALLEGRO_BITMAP *rarrowu;
ALLEGRO_BITMAP *rarrowl;
ALLEGRO_BITMAP *rarrowd;
ALLEGRO_BITMAP *rarrowr;
ALLEGRO_BITMAP *bslashu;
ALLEGRO_BITMAP *bslashd;
ALLEGRO_BITMAP *bslashr;
ALLEGRO_BITMAP *bslashl;
ALLEGRO_BITMAP *rslashu;
ALLEGRO_BITMAP *rslashr;
ALLEGRO_BITMAP *rslashl;
ALLEGRO_BITMAP *rslashd;
ALLEGRO_BITMAP *trofeu_tela;

typedef struct{
	short int esq_dir;
	short int flag;
	short int tipoPacote;
    short int HP;
    short int playerX;
    short int playerY;
    short int playerID;
    short int playerHit;
    short int timeFlecha;
    char playerName[16];
    char mov;
	bool situacao;	
    bool saiu;
}Player;

typedef struct{
	short int flagTarget;
	short int posicao[6][3];
	short int personagem[6][3];
	short int qtdPlayers;
	short int situacao[6];
	short int aux;
	short int teamTarget[2];
	char nicks[6][10];
	bool comecaJogo;
}Lobby;

typedef struct{
	short int qualTime;
	short int qualFlecha;
	short int qualAtaque;
	short int tag;
	short int HP;
	short int oldx;
	short int oldy;
	short int newx;
	short int newy;
	short int idMoved;
	short int botaBoneco;
	char olhando[9];
	char olhandoFlecha;
	char olhandoMagia;
}Moves;

//Funções centrais da Allegro
bool coreInit();
bool windowInit(int W, int H, char title[]);
bool inputInit();
bool fontInit();
void allegroEnd();

//Funções para controle da quantidade de Frames por Segundo (FPS)
void startTimer();
bool Blink();
void startTimerBlink();
double getTimer();
double getTimerBlink();
void FPSLimit();

//Função para carregamento de gráficos
bool loadGraphics();

#endif
