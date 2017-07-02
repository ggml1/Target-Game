#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2  
#define teamSelection 3 

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

    short int idsValidos, idsReady;
    short int qtdPlayers = 0;
    short int aux;
    struct msg_ret_t chegou;
    Player playerList[10], pacote, playersJogando[10];
    Moves mudaMatriz;
    Lobby nicknames;
    nicknames.comecaJogo = false;
    bool naoComeca = true;
    short int teamPos;

    short int i, j, x, y;

    for(i=0; i<6; i++){
        status[i] = false;
        //pĺayersJogando[i+3].HP = 20;
    }
    
    serverInit(6); // INICIALIZA O SERVER E PERMITE NO MAX. 6 CLIENTS
    
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
                    teamPos = 3 + chegou.client_id;
                    if(pacote.mov != 's') mudaMatriz.tag = 1;
                    switch(pacote.mov){
                        case 'u':
                            x = playersJogando[teamPos].playerX;
                            y = playersJogando[teamPos].playerY;
                            if(mapa[x - 1][y] == 0 || mapa[x - 1][y] == 2 || mapa[x - 1][y] == 55 || mapa[x - 1][y] == 56){
                                mudaMatriz.idMoved = teamPos;
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
                                mudaMatriz.olhando[teamPos] = 'u';
                                mapa[x][y] = teamPos;
                                playersJogando[teamPos].playerX--;
                            }
                            break;
                        case 'd':
                            x = playersJogando[teamPos].playerX;
                            y = playersJogando[teamPos].playerY;
                            if(mapa[x + 1][y] == 0 || mapa[x + 1][y] == 2 || mapa[x + 1][y] == 55 || mapa[x + 1][y] == 56){
                                mudaMatriz.idMoved = teamPos;
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
                                mudaMatriz.olhando[teamPos] = 'd';
                                mapa[x][y] = teamPos;
                                playersJogando[teamPos].playerX++;
                            }
                            break;
                        case 'l':
                            x = playersJogando[teamPos].playerX;
                            y = playersJogando[teamPos].playerY;
                            if(mapa[x][y - 1] == 0 || mapa[x][y - 1] == 2 || mapa[x][y - 1] == 55 || mapa[x][y - 1] == 56){
                                mudaMatriz.idMoved = teamPos;
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
                                mudaMatriz.olhando[teamPos] = 'l';
                                mapa[x][y] = teamPos;
                                playersJogando[teamPos].playerY--;
                            }
                            break;
                        case 'r':
                            x = playersJogando[teamPos].playerX;
                            y = playersJogando[teamPos].playerY;
                            if(mapa[x][y + 1] == 0 || mapa[x][y + 1] == 2 || mapa[x][y + 1] == 55 || mapa[x][y + 1] == 56){
                                mudaMatriz.idMoved = teamPos;
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
                                mudaMatriz.olhando[teamPos] = 'r';
                                mapa[x][y] = teamPos;
                                playersJogando[teamPos].playerY++;
                            }
                            break;
                        case 's':
                            playersJogando[teamPos].HP--;
                            printf("A vida atual do player eh %d\n", playersJogando[teamPos].HP);
                            if(playersJogando[teamPos].HP <= 0){
                                mudaMatriz.tag = 4;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), teamPos - 3);
                                disconnectClient(teamPos - 3);
                            } else{
                                mudaMatriz.tag = 2;
                                mudaMatriz.HP = playersJogando[teamPos].HP;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), chegou.client_id);
                            }
                            break;
                    }
                    if(mudaMatriz.tag == 1) broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    mudaMatriz.tag = 0;
                    break;

                case playerLogIn: /////////////////////////////////////////////////////////////////////2
                    //player logou no server. hora de mostrar onde ele fica.
                    mudaMatriz.tag = 3;
                    teamPos = 3 + chegou.client_id;
                    switch(teamPos){
                        case 3:
                            playersJogando[id+3].HP = 20;
                            playersJogando[3].playerX = 22;
                            playersJogando[3].playerY = 2;
                            mapa[22][2] = 3;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 2;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 2;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
                            break;
                        case 4:
                            playersJogando[id+3].HP = 20;
                            playersJogando[4].playerX = 22;
                            playersJogando[4].playerY = 3;
                            mapa[22][3] = 4;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 3;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 3;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
                            break;
                        case 5:
                            playersJogando[id+3].HP = 20;        
                            playersJogando[5].playerX = 22;
                            playersJogando[5].playerY = 4;
                            mapa[22][4] = 5;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 4;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 4;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
                            break;
                        case 6:
                            playersJogando[id+3].HP = 20;
                            playersJogando[6].playerX = 1;
                            playersJogando[6].playerY = 29;
                            mapa[1][29] = 6;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 29;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 29;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
                            break;
                        case 7:
                            playersJogando[id+3].HP = 20;
                            playersJogando[7].playerX = 1;
                            playersJogando[7].playerY = 28;
                            mapa[1][28] = 7;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 28;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 28;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
                            break;
                        case 8:
                            playersJogando[id+3].HP = 20;
                            playersJogando[8].playerX = 1;
                            playersJogando[8].playerY = 27;
                            mapa[1][27] = 8;
                            mudaMatriz.HP = 20;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 27;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 27;
                            mudaMatriz.idMoved = teamPos;
                            mudaMatriz.olhando[teamPos] = 'd';
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
                                    if(aux >= 4){ //&& naoComeca != false
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
                                    printf("NOME : %s, Time : %d \n", nicknames.nicks[i], pacote.mov);
                                }
                            }
                            printf("\n");
                        }
                        strcpy(nicknames.nicks[id], pacote.playerName);
                        for(i=0, idsValidos = 0, idsReady = 0; i<6; i++){
                            if(isValidId(i)){ // && status[i] == false
                                idsValidos++;
                                if(status[i] == true) idsReady++;
                            }
                        }
                        if(idsValidos == idsReady){
                            naoComeca = false;
                            nicknames.comecaJogo = true;
                        }
                        broadcast(&nicknames, sizeof(nicknames));
                    break;                                                                                                                                                                 
            }
            chegou.status = NO_MESSAGE;
        }
        if(chegou.status == DISCONNECT_MSG)
        {
            if(naoComeca == true){
                printf("o id %d quer se disconectar\n", chegou.client_id);
                posicao[id][0] = -1;
                posicao[id][1] = -1;
                posicao[id][2] = -1;
                
                broadcast(&nicknames, sizeof(nicknames));
                chegou.status = NO_MESSAGE;
            } else{
                x = playersJogando[3+chegou.client_id].playerX;
                y = playersJogando[3+chegou.client_id].playerY;
                (y > 15) ? (mapa[x][y] = 2, mudaMatriz.idMoved = 2) : (mapa[x][y] = 0, mudaMatriz.idMoved = 0); //ajeitar
                mudaMatriz.oldx = x;
                mudaMatriz.oldy = y;
                mudaMatriz.newx = x;
                mudaMatriz.newy = y;
                //mudaMatriz.idMoved = 0;
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