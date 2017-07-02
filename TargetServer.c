#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2  
#define teamSelection 3 

int mapa[24][32] = {{1,  1,   1,  1,  1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
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

int posicao[6][3] = {{-1, -1, -1},
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

    int qtdPlayers = 0;
    int aux;
    struct msg_ret_t chegou;
    Player playerList[10], pacote, playersJogando[10];
    Naksa pacoteProClient;
    Moves mudaMatriz;
    Lobby nicknames;
    nicknames.comecaJogo = false;

    int i, j, x, y;

    for(i=0; i<6; i++) status[i] = false;
    
    serverInit(6); // INICIALIZA O SERVER E PERMITE NO MAX. 6 CLIENTS
    
    while(!sair)
    {
        startTimer();
        int id = acceptConnection();
        bool naoComeca = false;
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
                    if(pacote.mov != 's') mudaMatriz.tag = 1;
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
                        case 's':
                            playersJogando[pacote.teamPos].HP--;
                            if(playersJogando[pacote.teamPos].HP == 0){
                                disconnectClient(pacote.teamPos - 3);
                            } else{
                                mudaMatriz.tag = 2;
                                mudaMatriz.HP = -1;
                                sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), chegou.client_id);
                            }
                            break;
                    }
                    if(mudaMatriz.tag == 1) broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    mudaMatriz.tag = 0;
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
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
                                    pacoteProClient.mapa[i][j] = mapa[i][j];
                                }
                            }
                            //sendMsgToClient(&pacote, sizeof(pacote), chegou.client_id);
                            break;
                    }
                    broadcast(&pacoteProClient, sizeof(pacoteProClient));
                    // for(i=0; i<6; i++){
                    //     if(isValidId(i) && i != chegou.client_id){
                    //         sendMsgToClient(&mudaMatriz, sizeof(mudaMatriz), i);
                    //     }
                    // }
                    break;
                
                case teamSelection:
                    if(pacote.flag == 0){
                        status[id] = pacote.situacao;
                        naoComeca = true;
                        if(pacote.saiu == true){
                            printf("entrei aqui no pacotesaiutrue\n");
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
                                    printf("NOME : %s, Time : %d \n", nicknames.nicks[i], pacote.mov);
                                }
                            }
                            printf("\n");
                        }
                        strcpy(nicknames.nicks[id], pacote.playerName);
                        for(i=0; i<6; i++){
                            if(isValidId(i) && status[i] == false){
                                naoComeca = true;
                            }
                        }
                        if(naoComeca == false) nicknames.comecaJogo = true;
                        broadcast(&nicknames, sizeof(nicknames));
                    break;
            }
            chegou.status = NO_MESSAGE;
        }
        if(chegou.status == DISCONNECT_MSG)
        {
            if(naoComeca == true){
                printf("o id %d quer se disconectar\n", chegou.client_id);
                posicao[id][pacote.mov + 1] = -1;
                
                broadcast(&nicknames, sizeof(nicknames));
                chegou.status = NO_MESSAGE;
            } else{
                x = playersJogando[3+chegou.client_id].playerX;
                y = playersJogando[3+chegou.client_id].playerY;
                (y > 15) ? (mapa[x][y] = 2, mudaMatriz.idMoved = 2) : (mapa[x][y] = 0, mudaMatriz.idMoved = 0);
                mudaMatriz.oldx = x;
                mudaMatriz.oldy = y;
                mudaMatriz.newx = x;
                mudaMatriz.newy = y;
                //mudaMatriz.idMoved = 0;
                broadcast(&mudaMatriz, sizeof(mudaMatriz));
                chegou.status = NO_MESSAGE;
            }
        }

        //if(naoComeca = true) broadcast(&nicknames, sizeof(nicknames));

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd(); // FINALIZA O ALLEGRO, FECHA A PAGINA E ACABA COM A FILA DE EVENTOS

    return 0;
    
}