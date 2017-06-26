#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#define playerFlagMSG 0
#define playerAction 1
#define playerLogIn 2

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
    Player playerList[6], pacote;

    
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
                case playerFlagMSG:
                    printf("%s se conectou com o id %d\n", pacote.playerName, chegou.client_id); //chegou.client_id = id;
                    playerList[id] = pacote;
                    break;
                case playerAction:
                    pacote.teamPos = 3 + chegou.client_id;
                    broadcast(&pacote, sizeof(pacote));
                    break;
                case playerLogIn:
                    pacote.teamPos = 3 + chegou.client_id;
                    switch(pacote.teamPos){
                        case 1:
                            pacote.playerX = 22;
                            pacote.playerY = 1;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                        case 2:
                            pacote.playerX = 21;
                            pacote.playerY = 1;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                        case 3:
                            pacote.playerX = 1;
                            pacote.playerY = 1;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                        case 4:
                            pacote.playerX = 1;
                            pacote.playerY = 30;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                        case 5:
                            pacote.playerX = 1;
                            pacote.playerY = 29;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                        case 6:
                            pacote.playerX = 1;
                            pacote.playerY = 28;
                            broadcast(&pacote, sizeof(pacote));
                            break;
                    }
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
