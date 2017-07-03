#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"

#define LARGURA 1024
#define ALTURA 768
#define TILE 32
#define FATOR 50

void menuGameJaComecou();
void menuErroSocket();
void menuErroServerFull();
void menuErroConnectionFailure();
void printaMapa(short int map[][32], Moves *Alteracoes);
void printaVida(short int HP);
void printaTelaConexao(char *nickname, char *ipOficial, int flag);
void printaCreditos();
void printaMenuOpcoes(int option);
void printaHelp();
void printaInicio();
void printaMenuTeamSelection(bool notReady);
void printaNomes(Lobby *nomes);
void mostraTelaMorte();
void printaAtaque(char direcao, short int newx, short int newy);
void printaFlechaRed(short int x, short int y, short int coeficiente, char direcao);
void printaFlechaBlue(short int x, short int y, short int coeficiente, char direcaoFlecha);

int main(int argc, char const *argv[]){

    //INICIALIZACAO DOS MOULOS CENTRAIS
    if (!coreInit())
        return -1;

    //CRIA A JANELA FISICA (WIDTH, HEIGHT, TITLE)
    if (!windowInit(LARGURA, ALTURA, "Target Client"))
        return -1;

    //INICIALIZA O TECLADO E O MOUSE
    if (!inputInit())
        return -1;

    //INICIALIZA AS FONTES
    if(!fontInit())
        return -1;

    //INICIALIZA OS GRAFICOS 
    if(!loadGraphics())
        return -1;
 
    //PROGRAM == false (CONDICAO PARA O JOGO FECHAR)
    char direcao;
    char direcaoFlecha[2];
    bool flecha[2];
    flecha[1] = false;
    flecha[0] = false;
	bool program = true;
    bool inicio = true;
    bool menu = false;
    bool menuConnection = false;
	bool help = false;
	bool credits = false;
    bool gameOn = false;
    bool teamSelection = false;
    bool pisca = false;
    bool receberMapa = false;
    short int coeficiente[2] = { 0 };
    short int xFlecha[2], yFlecha[2];
    short int count = 0;
    short int option = 0;
    short int conecta = 0;
    short int flag = 0;
    short int i, j, k;
    short int aux;    
    short int map[24][32] = {{1,  1,   1,  1,  1,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
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
    short int playerHP;

    Player pacoteClient, EU;
    Moves Alteracoes;
    Lobby nomes;
    
//-------------------------------------------------------------------------------------
    
    while(program)
    {
        while(inicio)
        {
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_DOWN){
    		      inicio = false;
    		      menu = true;
    		    }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
            }

            printaInicio();

            if(Blink() == 1) 
                pisca = !pisca;

            if(pisca)
                al_draw_textf(font_1, al_map_rgb(255,0,0), LARGURA/2 - 245, ALTURA/2 + 120, 0, "PRESS ANY KEY TO BEGIN");

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            FPSLimit();
        }
    // -------------------------------------------------------------------------------------------------------------------------------------------------
        while(menu)
        {
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                    switch(event.keyboard.keycode){
                        case ALLEGRO_KEY_UP:                     // APERTA PRA CIMA ELE SOBE, PERMITINDO O LOOP ENTRE OPCAO 1 E 4
                            option = (option + 3)%4;
                            break;
                        case ALLEGRO_KEY_DOWN:                   // APERTA PRA BAIXO ELE DESCE, PERMITINDO LOOP ENTRE OPCAO 4 E 1
                            option = (option + 1)%4;
                            break;
                        case ALLEGRO_KEY_ENTER:                  // APERTOU ENTER E VAI ESCOLHER A OPCAO 
                            switch(option){
                				case 0: 
                    				menuConnection = true;
                    				menu = false;
                				    break;
                				
                				case 1:
                    				help = true;
                    				menu = false;
                				    break;

                				case 2:
                    				credits = true;
                    				menu = false;
                				    break;
                		
                				case 3:
                				    allegroEnd();
                				    break;
    			            }
                            break;
                        case ALLEGRO_KEY_ESCAPE:
                            menu = false;
                            inicio = true;
                            break;
    		        }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();   // APERTOU O 'X' DA JANELA

                printaMenuOpcoes(option);

                al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
                FPSLimit();
            }
        }
    //--------------------------------------------------------------------------------------------------------------------------------------------------
        while(help)
        {
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                    menu = true;
                    help = false;
                }
        	   
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
            }

            printaHelp();    
        
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            FPSLimit();
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(credits)
        {
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                    ALLEGRO_EVENT event;
                    al_wait_for_event(eventsQueue, &event);
                    if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                        menu = true;
                        credits = false;
                    }
            	if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();

                printaCreditos();
                    
            	al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
                FPSLimit();
            }
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        char ipTemp[16] = { "" };
        char ipOficial[16] = { "0.0.0.0" };
        char nickname[16] = { "<seu_nick>" };
        char nicknameTemp[16] = { "" };
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(menuConnection)
        {
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                    if(flag == 0){ ///ESSA FLAG MARCA O FIM DO IP E COMECO DA INSERCAO DO NICKNAME
                        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                            menuConnection = false;
                            menu = true;
                        }
                        if(strlen(ipTemp) < 15){
                            char temp[] = {event.keyboard.unichar, '\0'};
                            if(event.keyboard.unichar >= '0' && event.keyboard.unichar <= '9'){
                                strcat(ipTemp, temp);
                                strcpy(ipOficial, ipTemp);
                            }
                            if(event.keyboard.unichar == '.'){
                                strcat(ipTemp, temp);
                                strcpy(ipOficial, ipTemp);
                            }
                        }
                        if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(ipTemp) != 0){
                            ipTemp[strlen(ipTemp) - 1] = '\0';
                            if(strlen(ipTemp) != 0) strcpy(ipOficial, ipTemp);
                            else strcpy(ipOficial, "0.0.0.0");
                        }
                        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && strlen(ipOficial) >= 9){
                            flag = 1;
                        }
                    } else{  ///ELSE DA FLAG (AQUI A FLAG == 1)
                        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                            menuConnection = false;
                            menu = true;
                            flag = 0;
                        }
                        if(strlen(nicknameTemp) < 15){
                            char nicktemp[] = {event.keyboard.unichar, '\0'};
                            if(event.keyboard.unichar >= 'A' && event.keyboard.unichar <= 'Z'){
                                strcat(nicknameTemp, nicktemp);
                                strcpy(nickname, nicknameTemp);
                            }
                            if(event.keyboard.unichar >= 'a' && event.keyboard.unichar <= 'z'){
                                strcat(nicknameTemp, nicktemp);
                                strcpy(nickname, nicknameTemp);
                            }
                            if(event.keyboard.unichar >= '0' && event.keyboard.unichar <= '9'){
                                strcat(nicknameTemp, nicktemp);
                                strcpy(nickname, nicknameTemp);
                            }
                        }
                        if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(nicknameTemp) != 0){
                            nicknameTemp[(strlen(nicknameTemp) - 1)] = '\0';
                            if(strlen(nicknameTemp) != 0) strcpy(nickname, nicknameTemp);
                            else strcpy(nickname, "<seu_nick>");
                        }
                        if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && strlen(nickname) >= 5){
                            conecta = connectToServer(ipOficial);
                            switch(conecta){
                                case -3:
                                    menuGameJaComecou();
                                    menu = true;
                                    menuConnection = false;
                                    flag = 0;
                                    strcpy(ipOficial, "0.0.0.0");
                                    strcpy(ipTemp, "");
                                    strcpy(nickname, "<seu_nick>");
                                    strcpy(nicknameTemp, "");
                                    break;
                                case -2:
                                    menuErroSocket();
                                    menu = true;
                                    menuConnection = false;
                                    flag = 0;
                                    strcpy(ipOficial, "0.0.0.0");
                                    strcpy(ipTemp, "");
                                    strcpy(nickname, "<seu_nick>");
                                    strcpy(nicknameTemp, "");
                                    break;
                                case -1:
                                    menuErroConnectionFailure();
                                    menu = true;
                                    menuConnection = false;
                                    flag = 0;
                                    strcpy(ipOficial, "0.0.0.0");
                                    strcpy(ipTemp, "");
                                    strcpy(nickname, "<seu_nick>");
                                    strcpy(nicknameTemp, "");
                                    break;
                                case 0:
                                    menuErroServerFull();
                                    menu = true;
                                    menuConnection = false;
                                    flag = 0;
                                    strcpy(ipOficial, "0.0.0.0");
                                    strcpy(ipTemp, "");
                                    strcpy(nickname, "<seu_nick>");
                                    strcpy(nicknameTemp, "");
                                    break;
                                case 1:
                                    menuConnection = false;
                                    teamSelection = true;
                                    //receberMapa = true;
                                    strcpy(pacoteClient.playerName, nickname);
                                    break;
                            }
                        }
                    } 
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();

                if(menuConnection == true) printaTelaConexao(nickname, ipOficial, flag);
                
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                FPSLimit();
            }
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        if(teamSelection){
            pacoteClient.tipoPacote = 3;
            pacoteClient.saiu = false;
            pacoteClient.mov = 0;
            pacoteClient.flag = 0;
            pacoteClient.esq_dir = 'm';
            pacoteClient.situacao = false;
            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));   ///PRIMEIRA MSG DO PLAYER PARA O SERVER, "OFICIALIZA" A CONEXAO E EVITA BUGS
            recvMsgFromServer(&nomes, WAIT_FOR_IT);
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(teamSelection){
            bool notReady = true;
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        if(pacoteClient.situacao == false){
                            pacoteClient.saiu = true;
                            pacoteClient.flag = 0;
                            pacoteClient.tipoPacote = 3;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            teamSelection = false;
                            menuConnection = true;
                            flag = 0;
                            strcpy(ipOficial, "0.0.0.0");
                            strcpy(ipTemp, "");
                            strcpy(nickname, "<seu_nick>");
                            strcpy(nicknameTemp, "");
                        } else{
                            pacoteClient.situacao = false;
                            pacoteClient.flag = 0;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                        }
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){
                        if(pacoteClient.situacao == false){
                            pacoteClient.flag = 0;

                            if(nomes.aux == 0) pacoteClient.mov--;
                            else pacoteClient.mov = pacoteClient.mov - 2;

                            pacoteClient.esq_dir = 'l';

                            if(pacoteClient.mov < -1) pacoteClient.mov = -1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                        }
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                        if(pacoteClient.situacao == false){
                            pacoteClient.flag = 0;

                            if(nomes.aux == 0) pacoteClient.mov++;
                            else pacoteClient.mov += 2;

                            pacoteClient.esq_dir = 'r';

                            if(pacoteClient.mov > 1) pacoteClient.mov = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                        }
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                        if(pacoteClient.mov != 0){
                            pacoteClient.flag = 1;
                            pacoteClient.situacao = true;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                        }
                    }
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    pacoteClient.saiu = true;
                    pacoteClient.flag = 0;
                    pacoteClient.tipoPacote = 3;
                    sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                    allegroEnd();
                }

                
            }

            if(recvMsgFromServer(&nomes, DONT_WAIT) == sizeof(nomes)){
                if(nomes.comecaJogo == true){
                    teamSelection = false;
                    receberMapa = true;
                } else{
                    for(i=0; i<6; i++){
                        for(j=0; j<3; j++){
                            printf("%d ", nomes.posicao[i][j]);
                        }
                        printf("\n");
                    }
                }
            }

            printaMenuTeamSelection(notReady);
            printaNomes(&nomes);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            FPSLimit();
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(receberMapa){
            pacoteClient.tipoPacote = 2;
            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
            printf("Recebi pra entrar!\n");
            if(recvMsgFromServer(&Alteracoes, WAIT_FOR_IT) == sizeof(Alteracoes)){
                if(Alteracoes.tag == 1){
                    if(Alteracoes.oldy < 16){
                        if((Alteracoes.oldy>=2 && Alteracoes.oldy<=4) && (Alteracoes.oldx>=20 && Alteracoes.oldx<=22)) map[Alteracoes.oldx][Alteracoes.oldy] = 55;
                        else map[Alteracoes.oldx][Alteracoes.oldy] = 0;
                    } else map[Alteracoes.oldx][Alteracoes.oldy] = 2;
                    map[Alteracoes.newx][Alteracoes.newy] = Alteracoes.idMoved;
                } else{
                    playerHP += Alteracoes.HP;
                }
                // printf("Posicoes:\nAntiga: %d %d\nNova: %d %d\n", Alteracoes.oldx, Alteracoes.oldy, Alteracoes.newx, Alteracoes.newy);
                // printf("Valor antigo: %d\n Valor novo: %d\n", map[Alteracoes.oldx][Alteracoes.oldy], map[Alteracoes.newx][Alteracoes.newy]);
            }
            for(i=0; i<6; i++) Alteracoes.olhando[3+i] = 'd';
            playerHP = 20;
            receberMapa = false;
            gameOn = true;
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(gameOn){

            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
                if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                    switch(event.keyboard.keycode){
                        case ALLEGRO_KEY_UP:
                            pacoteClient.mov = 'u';
                            pacoteClient.tipoPacote = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            break;
                        case ALLEGRO_KEY_DOWN:
                            pacoteClient.mov = 'd';
                            pacoteClient.tipoPacote = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            pacoteClient.mov = 'r';
                            pacoteClient.tipoPacote = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            break;
                        case ALLEGRO_KEY_LEFT:
                            pacoteClient.mov = 'l';
                            pacoteClient.tipoPacote = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            break;
                        case ALLEGRO_KEY_SPACE:
                            pacoteClient.mov = 's';
                            pacoteClient.tipoPacote = 1;
                            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                            break;
                    }
                }
            }
            if(recvMsgFromServer(&Alteracoes, DONT_WAIT) == sizeof(Alteracoes)){
                if(Alteracoes.tag == 1){
                    if(Alteracoes.oldy < 16){
                        if((Alteracoes.oldy>=2 && Alteracoes.oldy<=4) && (Alteracoes.oldx>=20 && Alteracoes.oldx<=22)) map[Alteracoes.oldx][Alteracoes.oldy] = 55;
                        else map[Alteracoes.oldx][Alteracoes.oldy] = 0;
                    } else{
                        if((Alteracoes.oldy>=27 && Alteracoes.oldy<=29) && (Alteracoes.oldx>=1 && Alteracoes.oldx<=3)) map[Alteracoes.oldx][Alteracoes.oldy] = 56; 
                        else map[Alteracoes.oldx][Alteracoes.oldy] = 2;
                    }
                    map[Alteracoes.newx][Alteracoes.newy] = Alteracoes.idMoved;
                }
                else if(Alteracoes.tag == 3){
                    map[Alteracoes.newx][Alteracoes.newy] = Alteracoes.idMoved;
                } else if(Alteracoes.tag == 4){
                    mostraTelaMorte();
                    gameOn = false;
                    menu = true;
                } else if(Alteracoes.tag == 5){
                    playerHP--;
                }
                // printf("Posicoes:\nAntiga: %d %d\nNova: %d %d\n", Alteracoes.oldx, Alteracoes.oldy, Alteracoes.newx, Alteracoes.newy);
                // printf("Valor antigo: %d\n Valor novo: %d\n", map[Alteracoes.oldx][Alteracoes.oldy], map[Alteracoes.newx][Alteracoes.newy]);
            }

            if(gameOn){
                printaMapa(map, &Alteracoes);
                printaVida(playerHP);
                if(Alteracoes.tag == 6){
                   direcao = Alteracoes.olhando[Alteracoes.idMoved];
                   printaAtaque(direcao, Alteracoes.newx, Alteracoes.newy);
                   Alteracoes.tag = 0; 
                } else if(Alteracoes.tag == 7){
                    flecha[Alteracoes.qualFlecha] = true;
                    xFlecha[Alteracoes.qualFlecha] = Alteracoes.newx;
                    yFlecha[Alteracoes.qualFlecha] = Alteracoes.newy;
                    direcaoFlecha[Alteracoes.qualFlecha] = Alteracoes.olhandoFlecha;
                    coeficiente[Alteracoes.qualFlecha] = 0;
                    Alteracoes.tag = 0;
                }
                for(i=0; i<2; i++){
                    if(flecha[i] == true){
                        coeficiente[i] += 10;
                        if(coeficiente[i] > 32){
                            coeficiente[i] = 0;
                            switch(direcaoFlecha[i]){
                                case 'u':
                                    xFlecha[i]--;
                                    break;
                                case 'd':
                                    xFlecha[i]++;
                                    break;
                                case 'r':
                                    yFlecha[i]++;
                                    break;
                                case 'l':
                                    yFlecha[i]--;
                                    break;
                            }
                        }
                        if(map[xFlecha[i]][yFlecha[i]] == 1 || map[xFlecha[i]][yFlecha[i]] == -1 || map[xFlecha[i]][yFlecha[i]] == 10 || map[xFlecha[i]][yFlecha[i]] == 30) flecha[i] = false;
                        switch(map[xFlecha[i]][yFlecha[i]]){
                            case 3:
                                pacoteClient.playerHit = 3;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                            case 4:
                                pacoteClient.playerHit = 4;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                            case 5:
                                pacoteClient.playerHit = 5;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                            case 6:
                                pacoteClient.playerHit = 6;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                            case 7:
                                pacoteClient.playerHit = 7;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                            case 8:
                                pacoteClient.playerHit = 8;
                                pacoteClient.tipoPacote = 10;
                                switch(direcaoFlecha[i]){
                                    case 'u':
                                        xFlecha[i]--;
                                        break;
                                    case 'd':
                                        xFlecha[i]++;
                                        break;
                                    case 'r':
                                        yFlecha[i]++;
                                        break;
                                    case 'l':
                                        yFlecha[i]--;
                                        break;
                                }
                                sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
                                break;
                        }
                        if(flecha[0] == true) printaFlechaRed(xFlecha[0], yFlecha[0], coeficiente[0], direcaoFlecha[0]);
                        if(flecha[1] == true) printaFlechaBlue(xFlecha[1], yFlecha[1], coeficiente[1], direcaoFlecha[1]);
                    }
                }
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            FPSLimit();
        }
    }
    return 0;       
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
//-----AQUI ESTA FORA DA MAIN - FUNCOES AUXILIARES!
//------------------------------------------------------------------------------------------------------------------------------------------------------
void printaFlechaBlue(short int x, short int y, short int coeficiente, char direcaoFlecha)
{
    switch(direcaoFlecha){
        case 'u':
            al_draw_bitmap(barrowu, TILE*y, TILE*x - coeficiente, 0);
            break;
        case 'd':
            al_draw_bitmap(barrowd, TILE*y, TILE*x + coeficiente, 0);
            break;
        case 'r':
            al_draw_bitmap(barrowr, TILE*y + coeficiente, TILE*x, 0);
            break;
        case 'l':
            al_draw_bitmap(barrowl, TILE*y - coeficiente, TILE*x, 0);
            break;
    }
}

void printaFlechaRed(short int x, short int y, short int coeficiente, char direcaoFlecha)
{
    switch(direcaoFlecha){
        case 'u':
            al_draw_bitmap(rarrowu, TILE*y, TILE*x - coeficiente, 0);
            break;
        case 'd':
            al_draw_bitmap(rarrowd, TILE*y, TILE*x + coeficiente, 0);
            break;
        case 'r':
            al_draw_bitmap(rarrowr, TILE*y + coeficiente, TILE*x, 0);
            break;
        case 'l':
            al_draw_bitmap(rarrowl, TILE*y - coeficiente, TILE*x, 0);
            break;
    }
}

void printaAtaque(char direcao, short int newx, short int newy)
{
    switch(direcao){
        case 'u':
            al_draw_bitmap(rslashu, TILE*newy - 32, TILE*newx - 32, 0);
            break;
        case 'd':
            al_draw_bitmap(rslashd, TILE*newy - 32, TILE*newx + 32, 0);
            break;
        case 'l':
            al_draw_bitmap(rslashl, TILE*newy - 32, TILE*newx - 32, 0);
            break;
        case 'r':
            al_draw_bitmap(rslashr, TILE*newy + 32, TILE*newx - 32, 0);
            break;
    }
}

void printaVida(short int HP)
{
    short int ciclo = 1, i, k;
    if(HP){
        for(i=0, k=0; i<HP; i++, k++){
            if(k == 4){
                k = 0;
                ciclo++;
            }
            switch(k){
                case 0:
                    al_draw_bitmap(heart_14, ciclo*TILE, 1*TILE, 0);
                    break;
                case 1:
                    al_draw_bitmap(heart_24, ciclo*TILE, 1*TILE, 0);
                    break;
                case 2:
                    al_draw_bitmap(heart_34, ciclo*TILE, 1*TILE, 0);
                    break;
                case 3:
                    al_draw_bitmap(heart_44, ciclo*TILE, 1*TILE, 0);
                    break;
            }
        }
    }
}

void mostraTelaMorte()
{
    int vdd = 1;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            printf("Player morreu.\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(telaMorte, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 345, ALLEGRO_ALIGN_CENTRE, "SORRY, BUT YOU DIED. HOPE YOU HAD FUN.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 391, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}

void menuGameJaComecou()
{
    int vdd = 1;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            printf("O jogo ja comecou! Nao foi possivel se conectar.\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 315, ALLEGRO_ALIGN_CENTRE, "OOPS! THE GAME HAS ALREADY STARTED.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 361, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}

void menuErroSocket()
{
    int vdd = 1;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            printf("Nao foi possivel criar a socket. Erro!\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 315, ALLEGRO_ALIGN_CENTRE, "OOPS! SOCKET ERROR.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 361, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}

void menuErroServerFull()
{
    int vdd = 1;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            printf("Server esta cheio. Erro!\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 315, ALLEGRO_ALIGN_CENTRE, "OOPS! SERVER IS FULL.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 361, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}

void menuErroConnectionFailure()
{   
    bool vdd = true;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            printf("Server nao esta up. Erro!\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = false;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 315, ALLEGRO_ALIGN_CENTRE, "OOPS! CONNECTION FAILURE.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 361, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}

void printaMapa(short int map[][32], Moves *Alteracoes)
{
    int i, j;
    for(i=0; i<24; i++){
                for(j=0; j<32; j++){
                        switch(map[i][j]){
                            case 2:
                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case -1:
                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                al_draw_bitmap_region(Dungeon_B, 5*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case 0:
                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case 1:
                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                al_draw_bitmap_region(Dungeon_B, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            //////////PLAYERS
                            case 3:
                                switch(Alteracoes->olhando[3]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=21) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }     
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            case 4:
                                switch(Alteracoes->olhando[4]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            case 5:
                                switch(Alteracoes->olhando[5]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            case 6:
                                switch(Alteracoes->olhando[6]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 0*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            case 7:
                                switch(Alteracoes->olhando[7]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 1*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            case 8:
                                switch(Alteracoes->olhando[8]){
                                    case 'u':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'd':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'l':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                    case 'r':
                                        if(j > 15){
                                            if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }else{
                                                al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        } else{
                                            if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            } else{
                                                al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(Shadow, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                                al_draw_bitmap_region(tileSet, 2*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            }
                                        }
                                        break;
                                }
                                break;
                            //////////PLAYERS

                            case 9:
                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                if(j == 26) al_draw_bitmap_region(TileC, 11*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 12*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case 91:
                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                if(j == 26) al_draw_bitmap_region(TileC, 11*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 12*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case 92:
                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                if(j == 26) al_draw_bitmap_region(TileC, 11*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 12*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case -9:
                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                if(j == 1) al_draw_bitmap_region(TileC, 9*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 10*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case -92:
                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                if(j == 1) al_draw_bitmap_region(TileC, 9*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 10*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;
                            case -91:
                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                if(j == 1) al_draw_bitmap_region(TileC, 9*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                else al_draw_bitmap_region(TileC, 10*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                break;

                            case 55:
                                al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                break;

                            case 56:
                                al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                break;

                            case 30: // LAPIDE, OK.  //LAPIDE NAO PEGA. VIROU CAVEIRA. ??
                                if(j > 15){
                                    if( ( j>=27 && j<=29 ) && (i>=1 && i<=3) ){
                                        al_draw_bitmap(blue, TILE*j, TILE*i, 0);
                                        al_draw_bitmap_region(Outside_B,   7*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    } else{
                                        al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                        al_draw_bitmap_region(Outside_B,   7*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    }
                                } else{
                                    if( (j >=2 && j <=4) && (i>=20 && i<=22) ){
                                        al_draw_bitmap(red, TILE*j, TILE*i, 0);
                                        al_draw_bitmap_region(Outside_B,   7*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    } else{
                                        al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                        al_draw_bitmap_region(Outside_B,   7*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    }
                                }

                            case 10:
                                if(j > 15){
                                    al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 10*TILE, 15*TILE, 32, 32, TILE*j, TILE*i, 0);
                                } else{
                                    al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 10*TILE, 14*TILE, 32, 32, TILE*j, TILE*i, 0);
                                }
                                break;
                            case 11:
                                if(j > 15){
                                    al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 8*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                } else{
                                    al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 8*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                }
                                break;
                            case 12:
                                if(j > 15){
                                    al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 8*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Inside_C, 15*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                } else{
                                    al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Dungeon_B, 8*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Inside_C, 14*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                }
                                break;
                            case 13:
                                if(j > 15){
                                    al_draw_bitmap_region(Dungeon_A2, 0*TILE, 10*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Inside_C, 15*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                } else{
                                    al_draw_bitmap_region(Dungeon_A2, 5*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(Inside_C, 14*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                }
                                break;    
                        }
                }
            }
}

void printaTelaConexao(char *nickname, char *ipOficial, int flag)
{
    al_draw_bitmap(menuNormal, 0, 0, 0);
    al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 + 20, LARGURA/2 + 320, ALTURA/2 + 70, al_map_rgb(255, 0, 0), 2);
    al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 - 100, LARGURA/2 + 320, ALTURA/2 - 50, al_map_rgb(255,0,0), 2);
    al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
    al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-10, ALTURA/2 - 150, ALLEGRO_ALIGN_CENTRE, "ENTER THE SERVER'S IP:");
    al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, ALTURA/2 - 94, ALLEGRO_ALIGN_CENTRE, ipOficial);
    al_draw_text(font_1, al_map_rgb(255, 0, 0), LARGURA/2-10, ALTURA/2 - 30, ALLEGRO_ALIGN_CENTRE, "ENTER YOUR NICKNAME:");
    al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, ALTURA/2 + 26, ALLEGRO_ALIGN_CENTRE, nickname);
    al_draw_text(font_2, al_map_rgb(0,0,0), LARGURA/2 + 5, ALTURA/2 + 80, ALLEGRO_ALIGN_CENTRE, "(MUST BE AT LEAST 5 CHARACTERS LONG)");
    al_draw_text(font_2, al_map_rgb(255, 0, 0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
    if(flag == 0) al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 - 95 , 0);
    else al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 + 25 , 0);
}

void printaCreditos()
{
    al_draw_bitmap(menuNormal, 0, 0, 0);
    al_draw_text(font_1, al_map_rgb(0, 0, 0), LARGURA / 2 , 50, ALLEGRO_ALIGN_CENTRE , "CREDITS");
    al_draw_text(font_1, al_map_rgb(255, 0, 0), 100, 200, ALLEGRO_ALIGN_LEFT , "01. STUDENTS");
    al_draw_text(font_2, al_map_rgb(0,0,0), 200, 270, ALLEGRO_ALIGN_CENTRE , "RAFAEL VASCONCELOS (RJAV)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 520, 270, ALLEGRO_ALIGN_CENTRE , "GABRIEL GOMES (GGML)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 200, 320, ALLEGRO_ALIGN_CENTRE , "MATHEUS CARACCIOLO (MCM2)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 520, 320, ALLEGRO_ALIGN_CENTRE , "ARTHUR HENRIQUE (AHTLC)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 200, 370, ALLEGRO_ALIGN_CENTRE , "GABRIEL FONSECA (GFSCA)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 520, 370, ALLEGRO_ALIGN_CENTRE , "BRUNO MOREIRA (BMMF)");
    al_draw_text(font_2, al_map_rgb(0,0,0), 800, 270, ALLEGRO_ALIGN_CENTRE , "VICTOR BRUNO (VBMS)");
    al_draw_text(font_1, al_map_rgb(255, 0, 0), 100, 440, ALLEGRO_ALIGN_LEFT , "02. MONITOR");
    al_draw_text(font_2, al_map_rgb(0,0,0), 365, 510, ALLEGRO_ALIGN_LEFT, "HEITOR SAMMUEL (HSCS)");
    al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA / 2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN TO MENU");
}

void printaMenuOpcoes(int option)
{
    al_draw_bitmap(menuOpcoes, 0, 0, 0);
    al_draw_text(font_1, al_map_rgb(255, 255, 255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE , "TARGET");
    al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) - 100, ALLEGRO_ALIGN_CENTRE , "START GAME");
    al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2), ALLEGRO_ALIGN_CENTRE , "HELP");
    al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) + 100, ALLEGRO_ALIGN_CENTRE , "CREDITS");
    al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) + 200, ALLEGRO_ALIGN_CENTRE , "EXIT GAME");
    switch(option){
        case 0:
            al_draw_bitmap(seta, LARGURA/2 - 232, ALTURA/2 - 96, 0);
            al_draw_bitmap(seta, LARGURA/2 + 178, ALTURA/2 - 96, -1);
            break;
        case 1:
            al_draw_bitmap(seta, LARGURA/2 - 232, ALTURA/2 + 2, 0);
            al_draw_bitmap(seta, LARGURA/2 + 178, ALTURA/2 + 2, -1);
            break;
        case 2:
            al_draw_bitmap(seta, LARGURA/2 - 232, ALTURA/2 + 102, 0);
            al_draw_bitmap(seta, LARGURA/2 + 178, ALTURA/2 + 102, -1);
            break;
        case 3:
            al_draw_bitmap(seta, LARGURA/2 - 232, ALTURA/2 + 202, 0);
            al_draw_bitmap(seta, LARGURA/2 + 178, ALTURA/2 + 202, -1);
            break;
    }
}

void printaHelp()
{
    al_draw_bitmap(menuNormal, 0, 0, 0);
    al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE , "HELP");
    al_draw_text(font_1, al_map_rgb(255,0,0), 100 , 120, ALLEGRO_ALIGN_LEFT , "01. MOVES");
    al_draw_text(font_2, al_map_rgb(0,0,0), 250, 200, ALLEGRO_ALIGN_CENTRE , "UP");
    al_draw_bitmap(upKey,     4*TILE + 20,  5*TILE + 15, 0);
    al_draw_bitmap(downKey,  15*TILE + 15,  5*TILE + 20, 0);
    al_draw_bitmap(leftKey,   4*TILE + 20,       8*TILE, 0);
    al_draw_bitmap(rightKey, 15*TILE + 15,  7*TILE + 25, 0);
    al_draw_bitmap(spaceTecla,     5*TILE, 11*TILE + 10, 0);
    al_draw_text(font_2, al_map_rgb(0,0,0), 600, 200, ALLEGRO_ALIGN_CENTRE , "DOWN");
    al_draw_text(font_2, al_map_rgb(0,0,0), 250, 270, ALLEGRO_ALIGN_CENTRE , "LEFT");
    al_draw_text(font_2, al_map_rgb(0,0,0), 600, 270, ALLEGRO_ALIGN_CENTRE , "RIGHT");
    al_draw_text(font_2, al_map_rgb(0,0,0), 250, 345, ALLEGRO_ALIGN_CENTRE , "SPACE BAR");
    al_draw_text(font_1, al_map_rgb(255,0,0), 100, 420, ALLEGRO_ALIGN_LEFT, "02. OBJECTIVE");
    al_draw_text(font_2, al_map_rgb(0,0,0), 200, 500, ALLEGRO_ALIGN_LEFT, "YOU MUST DEFEAT THE ENEMY TARGET");
    al_draw_text(font_2, al_map_rgb(0,0,0), 200, 540,  ALLEGRO_ALIGN_LEFT, "BEFORE THE ENEMY TEAM DEFEAT YOURS.");
    al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA / 2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN TO MENU");
}

void printaInicio()
{
    al_draw_bitmap(menuInicio, 0, 0, 0);
    al_draw_textf(font_3, al_map_rgb(255,255,255), LARGURA/2 - 181, 50, 0, "TARGET");
}

void printaMenuTeamSelection(bool notReady)
{
    al_draw_bitmap(menuNormal, 0, 0, 0);
    al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "TEAM LOBBY");
    al_draw_text(font_1, al_map_rgb(255,  0,  0), LARGURA/2 - 325, 110, ALLEGRO_ALIGN_CENTRE, "RED TEAM");
    al_draw_text(font_1, al_map_rgb(  0,  0,255), LARGURA/2 + 330, 110, ALLEGRO_ALIGN_CENTRE, "BLUE TEAM");
    al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
    al_draw_rectangle(LARGURA/3 - 310, 160,      LARGURA/3, 600, al_map_rgb(0, 0, 0), 5);
    al_draw_rectangle(LARGURA   - 345, 160,   LARGURA - 20, 600, al_map_rgb(0, 0, 0), 5);
    if(notReady) al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 610, ALLEGRO_ALIGN_CENTRE, "WAITING FOR PLAYERS...");
    else al_draw_text(font_2, al_map_rgb(0,255,0), LARGURA/2, 610, ALLEGRO_ALIGN_CENTRE, "EVERYONE IS READY!");
}

void printaNomes(Lobby *nomes)
{
    int i, j;
    for(i = 0; i < 6; i++){
        for(j = 0; j < 3; j++){
            switch(nomes->posicao[i][j]){
                case 0:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[0]);
                        if(nomes->situacao[0] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[0]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[0]);
                        if(nomes->situacao[0] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }

                    break;
                case 1:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[1]);
                        if(nomes->situacao[1] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[1]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[1]);
                        if(nomes->situacao[1] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25 , 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    break;
                case 2:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[2]);
                        if(nomes->situacao[2] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[2]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[2]);
                        if(nomes->situacao[2] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    break;
                case 3:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[3]);
                        if(nomes->situacao[3] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[3]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[3]);
                        if(nomes->situacao[3] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    break;
                case 4:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[4]);
                        if(nomes->situacao[4] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[4]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[4]);
                        if(nomes->situacao[4] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    break;
                case 5:
                    if(j == 0){
                        al_draw_textf(font_1,al_map_rgb(255,0,0), (LARGURA/3) - 160, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[5]);
                        if(nomes->situacao[5] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3) + 20, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    else if(j == 1)
                        al_draw_textf(font_1,al_map_rgb(0,0,0), (LARGURA/3)*2 - 175, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[5]);
                    else{
                        al_draw_textf(font_1,al_map_rgb(0,0,255), (LARGURA/3)*3 - 180, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "%s", nomes->nicks[5]);
                        if(nomes->situacao[5] == true)
                        al_draw_text(font_1,al_map_rgb(0,0,0),(LARGURA/3)*2 - 25, 170 + i*FATOR, ALLEGRO_ALIGN_CENTRE, "X");
                    }
                    break;
            }
        }
    }
}