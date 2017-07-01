#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2

int mapa[24][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,-1,-1  ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                    {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,9,56,56,56,9,-1},
                    {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,91,56,56,56,91,-1},
                    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,   0,2,2  ,2,2,-1,-1,2,2,2,2,92,56,56,56,92,-1},
                    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,   0,2,2  ,2,2,2,-1,-1,2,2,2,2,2,2,2,2,-1},
                    {1,1,1,0,0,10,0,0,0,0,0,1,0,0,0   ,0,2,  2,2,2,2,2,-1,2,2,2,2,2,2,2,2,-1},
                    {1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,2,2,2,2,2,-1},
                    {1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,   1,-1,-1  ,2,2,2,2,2,2,2,-1,2,2,2,2,2,-1},
                    {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,-1,-1,2,2,2,2,-1},
                    {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,-1,-1,2,2,2,-1},
                    {1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,   1,-1,-1  ,2,2,2,2,2,2,2,2,2,2,2,2,2,-1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,   11,11  ,-1,2,2,2,2,2,2,2,2,2,2,2,2,2,-1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,   12,12  ,-1,2,2,2,2,2,2,2,2,2,2,2,2,2,-1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,   13,13  ,-1,-1,-1,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1},
                    {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1},
                    {1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1},
                    {1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,   1,-1,-1  ,-1,-1,2,2,2,2,2,2,2,2,2,-1,-1,-1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,   0,2,2  ,2,-1,2,2,2,2,2,2,2,2,2,-1,-1,-1},
                    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,   0,2,2  ,2,-1,2,2,2,2,2,2,10,2,2,-1,-1,-1},
                    {1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1},
                    {1,-92,55,55,55,-92,0,0,0,0,1,1,0,0,0,   0,2,2  ,2,2,2,2,2,2,2,2,2,2,2,-1,-1,-1},
                    {1,-91,55,55,55,-91,0,0,0,0,0,0,0,   0,0,0  ,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                    {1,-9,55,55,55,-9,0,0,0,0,0,0,0,0,   0,0,2  ,2,2,2,2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   1,-1,-1  ,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

int main(){
    if(!coreInit()) //INICIA OS MODULOS PRINCIPAIS DO ALLEGRO
        return -1;
    
    if (!windowInit(150, 100, "SERVER")) // CRIA A JANELA DO SERVER
        return -1;
    
    if(!inputInit()) //SOMENTE O KEYBOARD ESTA SENDO INICIALIZADO
        return -1;

    if(!fontInit()) //FONTE BLACKWOOD CASTLE
        return -1;
    

    bool sair = false; // CONDICAO DE ENCERRAMENTO DO CLIENT 

    int qtdPlayers = 0; 
    struct msg_ret_t chegou;
    Player playerList[10], pacote, playersJogando[10];
    Moves mudaMatriz;

    int i, j, x, y;
    
    serverInit(6); // INICIALIZA O SERVER E PERMITE NO MAX. 4 CLIENTS
    
    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION) printf("Alguem se Conectou com ID %d\n", id);
        
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
                    pacote.teamPos = 3 + chegou.client_id;
                    switch(pacote.mov){
                        case 'u':
                            x = playersJogando[pacote.teamPos].playerX;
                            y = playersJogando[pacote.teamPos].playerY;
                            if(playersJogando[pacote.teamPos].playerX > 1 && (mapa[x - 1][y] == 0 || mapa[x - 1][y] == 2 || mapa[x - 1][y] == 55)){
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                    
                                    else mapa[x][y] = 0; 
                                }
                                else mapa[x][y] = 2;
                                x--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[pacote.teamPos] = 'u';
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerX--;
                            }
                            break;
                        case 'd':
                            x = playersJogando[pacote.teamPos].playerX;
                            y = playersJogando[pacote.teamPos].playerY;
                            if(playersJogando[pacote.teamPos].playerX < 22 && (mapa[x + 1][y] == 0 || mapa[x + 1][y] == 2 || mapa[x + 1][y] == 55)){
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                    
                                    else mapa[x][y] = 0; 
                                }
                                else mapa[x][y] = 2;
                                x++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[pacote.teamPos] = 'd';
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerX++;
                            }
                            break;
                        case 'l':
                            x = playersJogando[pacote.teamPos].playerX;
                            y = playersJogando[pacote.teamPos].playerY;
                            if(playersJogando[pacote.teamPos].playerY > 1 && (mapa[x][y - 1] == 0 || mapa[x][y - 1] == 2 || mapa[x][y - 1] == 55)){
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                    
                                    else mapa[x][y] = 0; 
                                }
                                else mapa[x][y] = 2;
                                y--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[pacote.teamPos] = 'l';
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerY--;
                            }
                            break;
                        case 'r':
                            x = playersJogando[pacote.teamPos].playerX;
                            y = playersJogando[pacote.teamPos].playerY;
                            if(playersJogando[pacote.teamPos].playerY < 30 && (mapa[x][y + 1] == 0 || mapa[x][y + 1] == 2 || mapa[x][y + 1] == 55)){
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                if(y < 16){
                                    if( (y >=2 && y <=4) && (x>=20 && x<=22) ) mapa[x][y] = 55;                                    
                                    else mapa[x][y] = 0; 
                                }
                                else mapa[x][y] = 2;
                                y++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mudaMatriz.olhando[pacote.teamPos] = 'r';
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerY++;
                            }
                            break;
                    }
                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    break;

                case playerLogIn: /////////////////////////////////////////////////////////////////////2
                    //player logou no server. hora de mostrar onde ele fica.
                    pacote.teamPos = 3 + chegou.client_id;
                    switch(pacote.teamPos){
                        case 3:
                            playersJogando[3].playerX = 22;
                            playersJogando[3].playerY = 2;
                            mapa[22][2] = 3;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 2;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 2;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                        case 4:
                            playersJogando[4].playerX = 22;
                            playersJogando[4].playerY = 3;
                            mapa[22][3] = 4;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 3;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 3;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                        case 5:
                            playersJogando[5].playerX = 22;
                            playersJogando[5].playerY = 4;
                            mapa[22][4] = 5;
                            mudaMatriz.oldx = 22;
                            mudaMatriz.oldy = 4;
                            mudaMatriz.newx = 22;
                            mudaMatriz.newy = 4;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                        case 6:
                            playersJogando[6].playerX = 1;
                            playersJogando[6].playerY = 30;
                            mapa[1][30] = 6;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 30;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 30;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                        case 7:
                            playersJogando[7].playerX = 1;
                            playersJogando[7].playerY = 29;
                            mapa[1][29] = 7;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 29;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 29;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                        case 8:
                            playersJogando[8].playerX = 1;
                            playersJogando[8].playerY = 28;
                            mapa[1][28] = 8;
                            mudaMatriz.oldx = 1;
                            mudaMatriz.oldy = 28;
                            mudaMatriz.newx = 1;
                            mudaMatriz.newy = 28;
                            mudaMatriz.idMoved = pacote.teamPos;
                            mudaMatriz.olhando[pacote.teamPos] = 'd';
                            for(i=0; i<24; i++){
                                for(j=0; j<32; j++){
                                    pacote.mapa[i][j] = mapa[i][j];
                                }
                            }
                            sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                    }
                    for(i=0; i<6; i++){
                        if(isValidId(i) && i != chegou.client_id){
                            sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), i);
                        }
                    }
                    break;
            }
            pacote.tipoPacote = -1;
        }
        if(chegou.status == DISCONNECT_MSG)
        {
            x = playersJogando[3+chegou.client_id].playerX;
            y = playersJogando[3+chegou.client_id].playerY;
            (y > 15) ? (mapa[x][y] = 2, mudaMatriz.idMoved = 2) : (mapa[x][y] = 0, mudaMatriz.idMoved = 0);
            mudaMatriz.oldx = x;
            mudaMatriz.oldy = y;
            mudaMatriz.newx = x;
            mudaMatriz.newy = y;
            //mudaMatriz.idMoved = 0;
            broadcast(&mudaMatriz, sizeof(mudaMatriz));
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd(); // FINALIZA O ALLEGRO, FECHA A PAGINA E ACABA COM A FILA DE EVENTOS

    return 0;
    
}
