#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2  
#define teamSelection 3 
#define playerAtingidoFlecha 10
#define playerAtingidoMagia 11

short int mapa[24][32] = {{1,  1,   1,  1,  1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                          {1,  1,   1,  1,  1,   1, 1, 1, 1, 1, 1, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  9, 56, 56, 56,  9, -1},
                          {1,  1,   1,  1,  1,   1, 1, 1, 1, 1, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 91, 56, 56, 56, 91, -1},
                          {1,  1,   1,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2, -1, -1,  2,  2,  2,  2, 92, 56, 56, 56, 92, -1},
                          {1,  1,   1,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2, -1, -1,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  1,   1,  0,  0,  10, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2, -1,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  1,   1,  0,  0,   0, 0, 0, 0, 0, 0, 1, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  1,   1,  0,  0,   0, 0, 0, 0, 0, 0, 1, 1, 1, 1,  1, -1, -1,  2,  2,  2,  2,  2,  2,  2, -1,  2,  2,  2,  2,  2, -1},
                          {1,  1,   1,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1,  2,  2,  2,  2, -1},
                          {1,  1,   1,  1,  1,   1, 1, 1, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1,  2,  2,  2, -1},
                          {1,  1,   1,  1,  1,   1, 1, 1, 0, 0, 0, 0, 1, 1, 1,  1, -1, -1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 11, 11, -1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 12, 12, -1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 13, -1, -1, -1,  2,  2,  2,  2, -1, -1, -1, -1, -1, -1, -1, -1},
                          {1,  0,   0,  0,  1,   1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1, -1, -1, -1, -1, -1, -1},
                          {1,  0,   0,  0,  0,   1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1, -1},
                          {1,  0,   0,  0,  0,   0, 1, 0, 0, 0, 0, 0, 0, 1, 1,  1, -1, -1, -1, -1,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2, -1,  2,  2,  2,  2,  2,  2,  2,  2,  2,- 1, -1, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 1, 0, 0, 0, 0, 0,  0,  2,  2,  2, -1,  2,  2,  2,  2,  2,  2, 10,  2,  2, -1, -1, -1},
                          {1,  0,   0,  0,  0,   0, 0, 0, 0, 1, 1, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1, -1},
                          {1, -92, 55, 55, 55, -92, 0, 0, 0, 0, 1, 1, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -1, -1, -1},
                          {1, -91, 55, 55, 55, -91, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                          {1,  -9, 55, 55, 55,  -9, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  2,  2,  2,  2,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                          {1,   1,  1,  1,  1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

short int posicao[6][3] = {{-1, -1, -1},
                           {-1, -1, -1},
                           {-1, -1, -1},
                           {-1, -1, -1},
                           {-1, -1, -1},
                           {-1, -1, -1}};

bool status[6];
 
int main(){
    if(!coreInit()) //INICIA OS MODULOS PRINCIPAIS DO ALLEGRO
        return -1;
    
    if (!windowInit(150, 100, "SERVER")) // CRIA A JANELA DO SERVER
        return -1;
    
    if(!inputInit()) //SOMENTE O KEYBOARD ESTA SENDO INICIALIZADO
        return -1;

    if(!fontInit()) //FONTES
        return -1;
    

    bool sair = false; // CONDICAO DE ENCERRAMENTO DO CLIENT 
    bool naoComeca = true;

    short int idsValidos, idsReady;
    short int qtdPlayers = 0;
    short int aux;
    short int teamPos[6], contadorVermelho = 0, contadorAzul = 0, coeficiente, xFlecha, yFlecha, xMagia, yMagia;
    short int i, j, x, y, p;

    struct msg_ret_t chegou;

    Player playerList[10], pacote, playersJogando[10];
    Moves mudaMatriz;
    Lobby nicknames;
    nicknames.flagTarget = 0;
    nicknames.comecaJogo = false;


    for(i=0; i<6; i++){
        status[i] = false;
    }
    
    serverInit(6); // INICIALIZA O SERVER E PERMITE NO MAX. 6 CLIENTS
    sair = false;
        
    while(!sair)
    {
        startTimer();
        int id;
        if(naoComeca == false) id = acceptConnection(0);
        else id = acceptConnection(1);
        if(id != NO_CONNECTION){
            printf("-----Alguem se conectou com ID %d.\n", id);
        }
        
        chegou = recvMsg(&pacote);
        if(chegou.status == MESSAGE_OK)
        {
            id = chegou.client_id;
            switch(pacote.tipoPacote){
                case playerFlagMSG: //0
                    printf("%s se conectou com o id %d\n", pacote.playerName, chegou.client_id); //chegou.client_id = id;
                    playerList[id] = pacote;
                    break;

                case playerAction: //1
                    if(pacote.mov != 's') mudaMatriz.tag = 1;
                    switch(pacote.mov){
                        case 'u':
                            x = playersJogando[teamPos[id]].playerX;
                            y = playersJogando[teamPos[id]].playerY;
                            if(mapa[x - 1][y] == 0 || mapa[x - 1][y] == 2 || mapa[x - 1][y] == 55 || mapa[x - 1][y] == 56){
                                mudaMatriz.botaBoneco = teamPos[id];
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                      
                                    else mapa[x][y] = 0; 
                                } else{
                                    if( (y >=27 && y <=29) && (x>=1 && x<=3) ) mapa[x][y] = 56;
                                    else mapa[x][y] = 2;
                                }
                                x--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[teamPos[id]] = 'u';
                                mapa[x][y] = teamPos[id];
                                playersJogando[teamPos[id]].playerX--;
                            }
                            break;
                        case 'd':
                            x = playersJogando[teamPos[id]].playerX;
                            y = playersJogando[teamPos[id]].playerY;
                            if(mapa[x + 1][y] == 0 || mapa[x + 1][y] == 2 || mapa[x + 1][y] == 55 || mapa[x + 1][y] == 56){
                                mudaMatriz.botaBoneco = teamPos[id];
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                      
                                    else mapa[x][y] = 0; 
                                } else{
                                    if( (y >=27 && y <=29) && (x>=1 && x<=3) ) mapa[x][y] = 56;
                                    else mapa[x][y] = 2;
                                }
                                x++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[teamPos[id]] = 'd';
                                mapa[x][y] = teamPos[id];
                                playersJogando[teamPos[id]].playerX++;
                            }
                            break;
                        case 'l':
                            x = playersJogando[teamPos[id]].playerX;
                            y = playersJogando[teamPos[id]].playerY;
                            if(mapa[x][y - 1] == 0 || mapa[x][y - 1] == 2 || mapa[x][y - 1] == 55 || mapa[x][y - 1] == 56){
                                mudaMatriz.botaBoneco = teamPos[id];
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                      
                                    else mapa[x][y] = 0; 
                                } else{
                                    if( (y >=27 && y <=29) && (x>=1 && x<=3) ) mapa[x][y] = 56;
                                    else mapa[x][y] = 2;
                                }
                                y--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[teamPos[id]] = 'l';
                                mapa[x][y] = teamPos[id];
                                playersJogando[teamPos[id]].playerY--;
                            }
                            break;
                        case 'r':
                            x = playersJogando[teamPos[id]].playerX;
                            y = playersJogando[teamPos[id]].playerY;
                            if(mapa[x][y + 1] == 0 || mapa[x][y + 1] == 2 || mapa[x][y + 1] == 55 || mapa[x][y + 1] == 56){
                                mudaMatriz.botaBoneco = teamPos[id];
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                     
                                    else mapa[x][y] = 0; 
                                } else{
                                    if( (y >=27 && y <=29) && (x>=1 && x<=3) ) mapa[x][y] = 56;
                                    else mapa[x][y] = 2;
                                }
                                y++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[teamPos[id]] = 'r';
                                mapa[x][y] = teamPos[id];
                                playersJogando[teamPos[id]].playerY++;
                            }
                            break;
                        case 's':
                            switch(teamPos[id]){
                                case 3:
                                    x = playersJogando[teamPos[id]].playerX;
                                    y = playersJogando[teamPos[id]].playerY;
                                    mudaMatriz.tag = 6;
                                    mudaMatriz.qualAtaque = 0;
                                    mudaMatriz.newx = x;
                                    mudaMatriz.newy = y;
                                    // aux = mudaMatriz.idMoved;
                                    mudaMatriz.idMoved = teamPos[id];
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    // mudaMatriz.idMoved = aux;
                                    mudaMatriz.tag = 5;
                                    switch(mudaMatriz.olhando[teamPos[id]]){
                                        case 'u':
                                            if(mapa[x-1][y-1] == 6 || mapa[x-1][y] == 6 || mapa[x-1][y+1] == 6){
                                                printf("**Player [%d] foi atingido pelo player [%d].\n", playersJogando[6].playerID, id);
                                                playersJogando[6].HP-=8;
                                                mudaMatriz.HP = playersJogando[6].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                            }
                                            if(mapa[x-1][y-1] == 7 || mapa[x-1][y] == 7 || mapa[x-1][y+1] == 7){
                                                playersJogando[7].HP-=8;
                                                mudaMatriz.HP = playersJogando[7].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);                                            
                                            }
                                            if(mapa[x-1][y-1] == 8 || mapa[x-1][y] == 8 || mapa[x-1][y+1] == 8){
                                                playersJogando[8].HP-=8;
                                                mudaMatriz.HP = playersJogando[8].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                            }
                                            break;
                                        case 'd':
                                            if(mapa[x+1][y+1] == 6 || mapa[x+1][y-1] == 6 || mapa[x+1][y] == 6){
                                                playersJogando[6].HP-=8;
                                                mudaMatriz.HP = playersJogando[6].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                            }
                                            if(mapa[x+1][y+1] == 7 || mapa[x+1][y-1] == 7 || mapa[x+1][y] == 7){
                                                playersJogando[7].HP-=8;
                                                mudaMatriz.HP = playersJogando[7].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);                                            
                                            }
                                            if(mapa[x+1][y+1] == 8 || mapa[x+1][y-1] == 8 || mapa[x+1][y] == 8){
                                                playersJogando[8].HP-=8;
                                                mudaMatriz.HP = playersJogando[8].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                            }
                                            break;
                                        case 'r':
                                            if(mapa[x][y+1] == 6 || mapa[x+1][y+1] == 6 || mapa[x-1][y+1] == 6){
                                                playersJogando[6].HP-=8;
                                                mudaMatriz.HP = playersJogando[6].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                            }
                                            if(mapa[x][y+1] == 7 || mapa[x+1][y+1] == 7 || mapa[x-1][y+1] == 7){
                                                playersJogando[7].HP-=8;
                                                mudaMatriz.HP = playersJogando[7].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);                                            
                                            }
                                            if(mapa[x][y+1] == 8 || mapa[x+1][y+1] == 8 || mapa[x-1][y+1] == 8){
                                                playersJogando[8].HP-=8;
                                                mudaMatriz.HP = playersJogando[8].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                            }
                                            break;
                                        case 'l':
                                            if(mapa[x][y-1] == 6 || mapa[x+1][y-1] == 6 || mapa[x-1][y-1] == 6){
                                                playersJogando[6].HP-=8;
                                                mudaMatriz.HP = playersJogando[6].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                            }
                                            if(mapa[x][y-1] == 7 || mapa[x+1][y-1] == 7 || mapa[x-1][y-1] == 7){
                                                playersJogando[7].HP-=8;
                                                mudaMatriz.HP = playersJogando[7].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);                                            
                                            }
                                            if(mapa[x][y-1] == 8 || mapa[x+1][y-1] == 8 || mapa[x-1][y-1] == 8){
                                                playersJogando[8].HP-=8;
                                                mudaMatriz.HP = playersJogando[8].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                            }  
                                            break; 
                                    }
                                    break;
                                case 4:
                                    xMagia = playersJogando[4].playerX;
                                    yMagia = playersJogando[4].playerY;
                                    mudaMatriz.tag = 8;
                                    mudaMatriz.newx = xMagia;
                                    mudaMatriz.newy = yMagia;
                                    mudaMatriz.olhandoMagia = mudaMatriz.olhando[4];
                                    mudaMatriz.qualFlecha = 0;
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    break;
                                case 5:
                                    xFlecha = playersJogando[5].playerX;
                                    yFlecha = playersJogando[5].playerY;
                                    mudaMatriz.tag = 7;
                                    mudaMatriz.newx = xFlecha;
                                    mudaMatriz.newy = yFlecha;
                                    mudaMatriz.olhandoFlecha = mudaMatriz.olhando[5];
                                    mudaMatriz.qualFlecha = 0;
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    break;
                                case 6:
                                    x = playersJogando[6].playerX;
                                    y = playersJogando[6].playerY;
                                    mudaMatriz.tag = 6;
                                    mudaMatriz.qualAtaque = 1;
                                    mudaMatriz.newx = x;
                                    mudaMatriz.newy = y;
                                    // aux = mudaMatriz.idMoved;
                                    mudaMatriz.idMoved = teamPos[id];
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    // mudaMatriz.idMoved = aux;
                                    mudaMatriz.tag = 5;
                                    switch(mudaMatriz.olhando[teamPos[id]]){
                                        case 'u':
                                            if(mapa[x-1][y-1] == 3 || mapa[x-1][y] == 3 || mapa[x-1][y+1] == 3){
                                                printf("**Player [%d] foi atingido pelo player 6.\n", playersJogando[3].playerID);
                                                playersJogando[3].HP-=8;
                                                mudaMatriz.HP = playersJogando[3].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);

                                            }
                                            if(mapa[x-1][y-1] == 4 || mapa[x-1][y] == 4 || mapa[x-1][y+1] == 4){
                                                playersJogando[4].HP-=8;
                                                mudaMatriz.HP = playersJogando[4].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID); 
                                            }
                                            if(mapa[x-1][y-1] == 5 || mapa[x-1][y] == 5 || mapa[x-1][y+1] == 5){
                                                playersJogando[5].HP-=8;
                                                mudaMatriz.HP = playersJogando[5].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                            }
                                            break;
                                        case 'd':
                                            if(mapa[x+1][y+1] == 3 || mapa[x+1][y-1] == 3 || mapa[x+1][y] == 3){
                                                playersJogando[3].HP-=8;
                                                mudaMatriz.HP = playersJogando[3].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);
                                            }
                                            if(mapa[x+1][y+1] == 4 || mapa[x+1][y-1] == 4 || mapa[x+1][y] == 4){
                                                playersJogando[4].HP-=8;
                                                mudaMatriz.HP = playersJogando[4].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID);                                            
                                            }
                                            if(mapa[x+1][y+1] == 5 || mapa[x+1][y-1] == 5 || mapa[x+1][y] == 5){
                                                playersJogando[5].HP-=8;
                                                mudaMatriz.HP = playersJogando[5].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                            }
                                            break;
                                        case 'r':
                                            if(mapa[x][y+1] == 3 || mapa[x+1][y+1] == 3 || mapa[x-1][y+1] == 3){
                                                playersJogando[3].HP-=8;
                                                mudaMatriz.HP = playersJogando[3].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);
                                            }
                                            if(mapa[x][y+1] == 4 || mapa[x+1][y+1] == 4 || mapa[x-1][y+1] == 4){
                                                playersJogando[4].HP-=8;
                                                mudaMatriz.HP = playersJogando[4].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID);                                            
                                            }
                                            if(mapa[x][y+1] == 5 || mapa[x+1][y+1] == 5 || mapa[x-1][y+1] == 5){
                                                playersJogando[5].HP-=8;
                                                mudaMatriz.HP = playersJogando[5].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                            }
                                            break;
                                        case 'l':
                                            if(mapa[x][y-1] == 3 || mapa[x+1][y-1] == 3 || mapa[x-1][y-1] == 3){
                                                playersJogando[3].HP-=8;
                                                mudaMatriz.HP = playersJogando[3].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);
                                            }
                                            if(mapa[x][y-1] == 4 || mapa[x+1][y-1] == 4 || mapa[x-1][y-1] == 4){
                                                playersJogando[4].HP-=8;
                                                mudaMatriz.HP = playersJogando[4].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID);                                            
                                            }
                                            if(mapa[x][y-1] == 5 || mapa[x+1][y-1] == 5 || mapa[x-1][y-1] == 5){
                                                playersJogando[5].HP-=8;
                                                mudaMatriz.HP = playersJogando[5].HP;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                            }  
                                            break; 
                                    }
                                    break;
                                case 7:
                                    xMagia = playersJogando[7].playerX;
                                    yMagia = playersJogando[7].playerY;
                                    mudaMatriz.tag = 8;
                                    mudaMatriz.newx = xMagia;
                                    mudaMatriz.newy = yMagia;
                                    mudaMatriz.olhandoMagia = mudaMatriz.olhando[7];
                                    mudaMatriz.qualFlecha = 1;
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    break;
                                case 8:
                                    xFlecha = playersJogando[8].playerX;
                                    yFlecha = playersJogando[8].playerY;
                                    mudaMatriz.tag = 7;
                                    mudaMatriz.newx = xFlecha;
                                    mudaMatriz.newy = yFlecha;
                                    mudaMatriz.olhandoFlecha = mudaMatriz.olhando[8];
                                    mudaMatriz.qualFlecha = 1;
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    break;
                            }
                            for(i=0; i<6; i++){
                                if(isValidId(i) && playersJogando[teamPos[i]].HP <= 0){
                                    printf("O player de ID [%d] morreu.\n", i);
                                    mudaMatriz.tag = 4;
                                    sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), i);
                                    if(teamPos[i] == nicknames.teamTarget[0]){
                                        for(j=0; j<6; j++){
                                            if(teamPos[j] <= 5 && isValidId(j)){
                                                mudaMatriz.tag = 21;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                                disconnectClient(j);
                                            } else if(teamPos[j] > 5 && isValidId(j)){
                                                mudaMatriz.tag = 20;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                                disconnectClient(j);
                                            }
                                    }
                                    serverReset();
                                    }
                                    if(teamPos[i] == nicknames.teamTarget[1]){
                                        for(j=0; j<6; j++){
                                            if(teamPos[j] <= 5 && isValidId(j)){
                                                mudaMatriz.tag = 20;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                                disconnectClient(j);
                                            } else if(teamPos[j] > 5 && isValidId(j)){
                                                mudaMatriz.tag = 21;
                                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                                disconnectClient(j);
                                            }
                                    }
                                    serverReset();
                                    } else{
                                        x = playersJogando[teamPos[i]].playerX;
                                        y = playersJogando[teamPos[i]].playerY;
                                        mudaMatriz.tag = 3;
                                        mudaMatriz.newx = x;
                                        mudaMatriz.newy = y;
                                        mudaMatriz.botaBoneco = 10;
                                        disconnectClient(i);
                                        broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                    }
                                }
                            }
                            break;
                    }
                    if(mudaMatriz.tag == 1) broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    mudaMatriz.tag = 0;
                    break;

                case playerLogIn: /////////////////////////////////////////////////////////////////////2
                    //player logou no server. hora de mostrar onde ele fica.
                    mudaMatriz.tag = 3;
                    switch(teamPos[id]){
                        case 3:
                            playersJogando[3].HP = 48;
                            playersJogando[3].playerX = 22;
                            playersJogando[3].playerY = 2;
                            mapa[22][2] = 3;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 2;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 2;
                            mudaMatriz.botaBoneco = 3;
                            mudaMatriz.olhando[3] = 'd';
                            break;
                        case 4:
                            playersJogando[4].HP = 48;
                            playersJogando[4].playerX = 22;
                            playersJogando[4].playerY = 3;
                            mapa[22][3] = 4;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 3;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 3;
                            mudaMatriz.botaBoneco = 4;
                            mudaMatriz.olhando[4] = 'd';
                            break;
                        case 5:
                            playersJogando[5].HP = 48;        
                            playersJogando[5].playerX = 22;
                            playersJogando[5].playerY = 4;
                            mapa[22][4] = 5;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 4;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 4;
                            mudaMatriz.botaBoneco = 5;
                            mudaMatriz.olhando[5] = 'd';
                            break;
                        case 6:
                            playersJogando[6].HP = 48;
                            playersJogando[6].playerX = 1;
                            playersJogando[6].playerY = 29;
                            mapa[1][29] = 6;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 29;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 29;
                            mudaMatriz.botaBoneco = 6;
                            mudaMatriz.olhando[6] = 'd';
                            break;
                        case 7:
                            playersJogando[7].HP = 48;
                            playersJogando[7].playerX = 1;
                            playersJogando[7].playerY = 28;
                            mapa[1][28] = 7;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 28;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 28;
                            mudaMatriz.botaBoneco = 7;
                            mudaMatriz.olhando[7] = 'd';
                            break;
                        case 8:
                            playersJogando[8].HP = 48;
                            playersJogando[8].playerX = 1;
                            playersJogando[8].playerY = 27;
                            mapa[1][27] = 8;
                            mudaMatriz.HP = 48;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 27;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 27;
                            mudaMatriz.botaBoneco = 8;
                            mudaMatriz.olhando[8] = 'd';
                            break;
                    }
                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    break;
                
                case teamSelection:
                    if(pacote.flag == 0){
                        status[id] = pacote.situacao;
                        naoComeca = true;
                        if(pacote.saiu == true){
                            disconnectClient(chegou.client_id);
                            for(i=0; i<6; i++){
                                for(j=0; j<3; j++){
                                    if(posicao[i][j] == chegou.client_id){
                                        posicao[i][j] = -1;
                                    }
                                    nicknames.posicao[i][j] = posicao[i][j];
                                }
                            }
                            broadcast(&nicknames, sizeof(nicknames));
                        } else{
                            status[id] = pacote.situacao;
                            switch(pacote.mov){
                                case -1:
                                    aux = 0;
                                    for(i=0; i<6; i++){
                                        if(posicao[i][0] == -1) aux++;
                                    }
                                    if(aux >= 4){
                                        nicknames.aux = 0;
                                        posicao[id][0] = id;
                                        posicao[id][1] = -1;
                                    } else nicknames.aux = 1;
                                    break;
                                case 0:
                                    nicknames.aux = 0;
                                    posicao[id][1] = id;
                                    if(pacote.esq_dir == 'r') posicao[id][0] = -1;
                                    else if(pacote.esq_dir == 'l') posicao[id][2] = -1;
                                    break;
                                case 1:
                                    aux = 0;
                                    for(i=0; i<6; i++){
                                        if(posicao[i][2] == -1) aux++;
                                    }
                                    if(aux >= 4){
                                        nicknames.aux = 0;
                                        posicao[id][2] = id;
                                        posicao[id][1] = -1;
                                    } else nicknames.aux = 1;
                                    break;
                            }
                        }
                    } else if(pacote.flag == 1){
                        status[id] = pacote.situacao;
                        }
                        for(i=0; i<6; i++){
                            nicknames.situacao[i] = status[i];
                            if(!isValidId(i)){
                                for(j=0; j<3; j++) nicknames.posicao[i][j] = -1;
                            } else{
                                for(j=0; j<3; j++){
                                    printf("Nicknames ta recebendo [%d] na posicao [%d][%d]\n", posicao[i][j], i, j);
                                    nicknames.posicao[i][j] = posicao[i][j];
                                    printf("NOME -> %s, Time -> %d \n", nicknames.nicks[i], pacote.mov);
                                }
                            }
                            printf("\n");
                        }
                        strcpy(nicknames.nicks[id], pacote.playerName);
                        for(i=0, idsValidos = 0, idsReady = 0; i<6; i++){
                            if(isValidId(i)){
                                idsValidos++;
                                if(status[i] == true) idsReady++;
                            }
                        }
                        if(idsValidos == idsReady){
                            naoComeca = false;
                            nicknames.comecaJogo = true;
                            for(i=0, contadorVermelho = 0, contadorAzul = 0; i<6; i++){
                                for(j=0; j<3; j++){
                                    if(posicao[i][j] != -1){
                                        if(j == 0){
                                            //vermelho
                                            contadorVermelho++;
                                            switch(contadorVermelho){
                                                case 1:
                                                    teamPos[posicao[i][j]] = 3;
                                                    playersJogando[3].playerID = posicao[i][j];
                                                    break;
                                                case 2:
                                                    teamPos[posicao[i][j]] = 4;
                                                    playersJogando[4].playerID = posicao[i][j];
                                                    break;
                                                case 3:
                                                    teamPos[posicao[i][j]] = 5;
                                                    playersJogando[5].playerID = posicao[i][j];
                                                    break;
                                            }
                                        } else if(j == 2){
                                            //azul
                                            contadorAzul++;
                                            switch(contadorAzul){
                                                case 1:
                                                    teamPos[posicao[i][j]] = 6;
                                                    playersJogando[6].playerID = posicao[i][j];
                        
                                                    break;
                                                case 2:
                                                    teamPos[posicao[i][j]] = 7;
                                                    playersJogando[7].playerID = posicao[i][j];
                        
                                                    break;
                                                case 3:
                                                    teamPos[posicao[i][j]] = 8;
                                                    playersJogando[8].playerID = posicao[i][j];
                        
                                                    break;
                                            }
                                        }
                                    }
                                }
                            }
                            srand(time(NULL)); /////////////////////////////////////////////////////////////////////////////////////
                            if(contadorVermelho > 0){
                                nicknames.flagTarget = 1;
                                nicknames.teamTarget[0] = 3 + rand()%contadorVermelho;
                                printf("O rand do red deu %d\nO contador vermelho eh %d\n.", nicknames.teamTarget[0], contadorVermelho);
                                for(p=0; p<contadorVermelho; p++){
                                    sendMsgToClient(&nicknames, sizeof(nicknames), playersJogando[p+3].playerID);
                                }
                            }
                            srand(time(NULL));
                            if(contadorAzul > 0){
                                nicknames.flagTarget = 2;
                                nicknames.teamTarget[1] = 6 + rand()%contadorAzul;
                                printf("O rand do red deu %d\nO contador azul eh %d\n.", nicknames.teamTarget[1], contadorAzul);
                                for(p=0; p<contadorAzul; p++){
                                    sendMsgToClient(&nicknames, sizeof(nicknames), playersJogando[p+6].playerID);
                                }
                            }
                        } else{
                            for(i=0, contadorAzul = 0, contadorVermelho = 0; i<6; i++){
                                for(j=0; j<3; j++){
                                    if(posicao[i][j] != -1){
                                        if(j == 0){
                                            contadorVermelho++;
                                            switch(contadorVermelho){
                                                case 1:
                                                    nicknames.personagem[i][j] = 3;
                                                    break;
                                                case 2:
                                                    nicknames.personagem[i][j] = 4;
                                                    break;
                                                case 3:
                                                    nicknames.personagem[i][j] = 5;
                                                    break;
                                            }
                                        } else if(j == 2){
                                            contadorAzul++;
                                            switch(contadorAzul){
                                                case 1:
                                                    nicknames.personagem[i][j] = 6;
                                                    break;
                                                case 2:
                                                    nicknames.personagem[i][j] = 7;
                                                    break;
                                                case 3:
                                                    nicknames.personagem[i][j] = 8;
                                                    break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        broadcast(&nicknames, sizeof(nicknames));
                    break;                                                                                                                                                                 
                
                case playerAtingidoFlecha:
                        switch(pacote.playerHit){
                            case 3:
                                playersJogando[3].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[3].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);
                                break;
                            case 4:
                                playersJogando[4].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[4].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID);
                                break;
                            case 5:
                                playersJogando[5].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[5].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                break;
                            case 6:
                                playersJogando[6].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[6].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                break;
                            case 7:
                                playersJogando[7].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[7].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);
                                break;
                            case 8:
                                playersJogando[8].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[8].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                break;
                        
                    }
                    for(i=0; i<6; i++){
                        if(isValidId(i) && playersJogando[teamPos[i]].HP <= 0){
                            printf("O player de ID [%d] morreu.\n", i);
                            mudaMatriz.tag = 4;
                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), i);
                            if(teamPos[i] == nicknames.teamTarget[0]){
                                for(j=0; j<6; j++){
                                    if(teamPos[j] <= 5 && isValidId(j)){
                                        mudaMatriz.tag = 21;
                                        sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                        disconnectClient(j);
                                    } else if(teamPos[j] > 5 && isValidId(j)){
                                        mudaMatriz.tag = 20;
                                        sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                        disconnectClient(j);
                                    }
                                }
                                serverReset();
                            }
                            if(teamPos[i] == nicknames.teamTarget[1]){
                                for(j=0; j<6; j++){
                                    if(teamPos[j] <= 5 && isValidId(j)){
                                        mudaMatriz.tag = 20;
                                        sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                        disconnectClient(j);
                                    } else if(teamPos[j] > 5 && isValidId(j)){
                                        mudaMatriz.tag = 21;
                                        sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                        disconnectClient(j);
                                    }
                                }
                                serverReset();
                            } else{
                                x = playersJogando[teamPos[i]].playerX;
                                y = playersJogando[teamPos[i]].playerY;
                                mudaMatriz.tag = 3;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mapa[x][y] = 10;
                                mudaMatriz.botaBoneco = 10;
                                disconnectClient(i);
                                broadcast(&mudaMatriz, sizeof(mudaMatriz));
                            }
                        }
                    }
                    break;
            
                case playerAtingidoMagia:
                        switch(pacote.playerHit){
                            case 3:
                                playersJogando[3].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[3].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[3].playerID);
                                break;
                            case 4:
                                playersJogando[4].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[4].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[4].playerID);
                                break;
                            case 5:
                                playersJogando[5].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[5].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[5].playerID);
                                break;
                            case 6:
                                playersJogando[6].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[6].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[6].playerID);
                                break;
                            case 7:
                                playersJogando[7].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[7].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[7].playerID);
                                break;
                            case 8:
                                playersJogando[8].HP-=2;
                                mudaMatriz.tag = 5;
                                mudaMatriz.HP = playersJogando[8].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), playersJogando[8].playerID);
                                break;
                        }
                        for(i=0; i<6; i++){
                            if(isValidId(i) && playersJogando[teamPos[i]].HP <= 0){
                                printf("O player de ID [%d] morreu.\n", i);
                                mudaMatriz.tag = 4;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), i);
                                if(teamPos[i] == nicknames.teamTarget[0]){
                                    for(j=0; j<6; j++){
                                        if(teamPos[j] <= 5 && isValidId(j)){
                                            mudaMatriz.tag = 21;
                                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                            disconnectClient(j);
                                        } else if(teamPos[j] > 5 && isValidId(j)){
                                            mudaMatriz.tag = 20;
                                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                            disconnectClient(j);
                                        }
                                }
                                serverReset();
                                }
                                if(teamPos[i] == nicknames.teamTarget[1]){
                                    for(j=0; j<6; j++){
                                        if(teamPos[j] <= 5 && isValidId(j)){
                                            mudaMatriz.tag = 20;
                                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                            disconnectClient(j);
                                        } else if(teamPos[j] > 5 && isValidId(j)){
                                            mudaMatriz.tag = 21;
                                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), j);
                                            disconnectClient(j);
                                        }
                                }
                                serverReset();
                                } else{
                                    x = playersJogando[teamPos[i]].playerX;
                                    y = playersJogando[teamPos[i]].playerY;
                                    mudaMatriz.tag = 3;
                                    mudaMatriz.newx = x;
                                    mudaMatriz.newy = y;
                                    mapa[x][y] = 10;
                                    mudaMatriz.botaBoneco = 10;
                                    disconnectClient(i);
                                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                                }
                            }
                    }
                    break;
            }
            chegou.status = NO_MESSAGE;
        }
        if(chegou.status == DISCONNECT_MSG)
        {
            if(naoComeca == true){
                printf("O player de ID %d quer se desconectar.\n", chegou.client_id);

                posicao[id][0] = -1;
                posicao[id][1] = -1;
                posicao[id][2] = -1;
                
                broadcast(&nicknames, sizeof(nicknames));
                chegou.status = NO_MESSAGE;
            } else{
                x = playersJogando[teamPos[chegou.client_id]].playerX;
                y = playersJogando[teamPos[chegou.client_id]].playerY;
                mudaMatriz.tag = 3;
                mudaMatriz.newx = x;
                mudaMatriz.newy = y;
                mudaMatriz.botaBoneco = 30;
                mapa[x][y] = 30;
                broadcast(&mudaMatriz, sizeof(mudaMatriz));
                chegou.status = NO_MESSAGE;
            }
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd(); // FINALIZA O ALLEGRO, FECHA A PAGINA E ACABA COM A FILA DE EVENTOS

    return 0;
    
}