#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2

int mapa[24][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

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
    Player playerList[6], pacote, playersJogando[10];
    Moves mudaMatriz;

    int i, j, x, y;
    
    serverInit(6); // INICIALIZA O SERVER E PERMITE NO MAX. 4 CLIENTS
    
    while(!sair)
    {
        startTimer();
        int id = acceptConnection();

        if(id != NO_CONNECTION)
            printf("Alguem se Conectou com ID %d\n", id);
        
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
                            if(playersJogando[pacote.teamPos].playerX > 1){
                                x = playersJogando[pacote.teamPos].playerX;
                                y = playersJogando[pacote.teamPos].playerY;
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                mapa[x][y] = 0;
                                x--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerX--;
                            }
                            break;
                        case 'd':
                            if(playersJogando[pacote.teamPos].playerX < 22){
                                x = playersJogando[pacote.teamPos].playerX;
                                y = playersJogando[pacote.teamPos].playerY;
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                mapa[x][y] = 0;
                                x++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerX++;
                            }
                            break;
                        case 'l':
                            if(playersJogando[pacote.teamPos].playerY > 1){
                                x = playersJogando[pacote.teamPos].playerX;
                                y = playersJogando[pacote.teamPos].playerY;
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                mapa[x][y] = 0;
                                y--;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerY--;
                            }
                            break;
                        case 'r':
                            if(playersJogando[pacote.teamPos].playerY < 30){
                                x = playersJogando[pacote.teamPos].playerX;
                                y = playersJogando[pacote.teamPos].playerY;
                                mudaMatriz.idMoved = pacote.teamPos;
                                mudaMatriz.oldx = x;
                                mudaMatriz.oldy = y;
                                mapa[x][y] = 0;
                                y++;
                                mudaMatriz.newx = x;
                                mudaMatriz.newy = y;
                                mapa[x][y] = pacote.teamPos;
                                playersJogando[pacote.teamPos].playerY++;
                            }
                            break;
                    }
                    broadcast(&mudaMatriz, sizeof(mudaMatriz));
                    break;

                case playerLogIn: //2
                    //player logou no server. hora de mostrar onde ele fica.
                    pacote.teamPos = 3 + chegou.client_id;
                    switch(pacote.teamPos){
                        case 3:
                            playersJogando[3].playerX = 22;
                            playersJogando[3].playerY = 1;
                            mapa[22][1] = 3;
                            break;
                        case 4:
                            playersJogando[4].playerX = 21;
                            playersJogando[4].playerY = 1;
                            mapa[21][1] = 4;
                            break;
                        case 5:
                            playersJogando[5].playerX = 1;
                            playersJogando[5].playerY = 1;
                            mapa[1][1] = 5;;
                            break;
                        case 6:
                            playersJogando[6].playerX = 1;
                            playersJogando[6].playerY = 30;
                            mapa[1][30] = 6;
                            break;
                    }
                    for(i=0; i<24; i++){
                        for(j=0; j<32; j++){
                            pacote.mapa[i][j] = mapa[i][j];
                        }
                    }
                    broadcast(&pacote, sizeof(pacote));
                    break;
            }
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }

    allegroEnd(); // FINALIZA O ALLEGRO, FECHA A PAGINA E ACABA COM A FILA DE EVENTOS

    return 0;
    
}
