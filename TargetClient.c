#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"

#define LARGURA 1024
#define ALTURA 768
#define TILE 32

void menuErroSocket();
void menuErroServerFull();
void menuErroConnectionFailure();

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
    int count = 0;
    int option = 0;
    int conecta = 0;
    int flag = 0;
    int i, j, l = 0;
    int aux;    
    int map[24][32];

    //struct msg_ret_t chegouSv;
    Player pacoteClient, pacoteDoServer, EU;
    Moves Alteracoes;
    
//-------------------------------------------------------------------------------------
    
while (program)
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
		al_draw_bitmap(menuInicio, 0, 0, 0);
        al_draw_textf(font_3, al_map_rgb(255,255,255), LARGURA/2 - 181, 50, 0, "TARGET");
        if(Blink() == 1) 
            pisca = !pisca;

        if(pisca)
            al_draw_textf(font_1, al_map_rgb(255,0,0), LARGURA/2 - 245, ALTURA/2 + 120, 0, "PRESS ANY KEY TO BEGIN");

        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }
    
// ---------------------------------------------------------------------------------------------------------------------------------------------
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
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
            FPSLimit();
        }
    }
//-----------------------------------------------------------------------------------------------------------------------------------------------------  

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

        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE , "HELP");
        al_draw_text(font_1, al_map_rgb(255,0,0), 100 , 120, ALLEGRO_ALIGN_LEFT , "01. MOVES");
        al_draw_text(font_2, al_map_rgb(0,0,0), 200, 200, ALLEGRO_ALIGN_CENTRE , "UP");
        al_draw_text(font_2, al_map_rgb(0,0,0), 600, 200, ALLEGRO_ALIGN_CENTRE , "DOWN");
        al_draw_text(font_2, al_map_rgb(0,0,0), 200, 270, ALLEGRO_ALIGN_CENTRE , "LEFT");
        al_draw_text(font_2, al_map_rgb(0,0,0), 600, 270, ALLEGRO_ALIGN_CENTRE , "RIGHT");
        al_draw_text(font_2, al_map_rgb(0,0,0), 200, 340, ALLEGRO_ALIGN_CENTRE , "SPACE BAR");
        al_draw_text(font_1, al_map_rgb(255,0,0), 100, 420, ALLEGRO_ALIGN_LEFT, "02. OBJECTIVE");
        al_draw_text(font_2, al_map_rgb(0,0,0), 200, 500, ALLEGRO_ALIGN_LEFT, "YOU MUST DEFEAT THE ENEMY TARGET");
        al_draw_text(font_2, al_map_rgb(0,0,0), 200, 540,  ALLEGRO_ALIGN_LEFT, "BEFORE THE ENEMY TEAM DEFEAT YOURS.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA / 2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN TO MENU");
    
    
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
        FPSLimit();
    }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
              	al_draw_bitmap(menuNormal, 0, 0, 0);
            	al_draw_text(font_1, al_map_rgb(0, 0, 0), LARGURA / 2 , 50, ALLEGRO_ALIGN_CENTRE , "CREDITS");
            	al_draw_text(font_1, al_map_rgb(255, 0, 0), 100, 200, ALLEGRO_ALIGN_LEFT , "01. STUDENTS");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 200, 270, ALLEGRO_ALIGN_CENTRE , "RAFAEL VASCONCELOS");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 520, 270, ALLEGRO_ALIGN_CENTRE , "GABRIEL GOMES");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 200, 320, ALLEGRO_ALIGN_CENTRE , "MATHEUS CARACCIOLO");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 520, 320, ALLEGRO_ALIGN_CENTRE , "ARTHUR BARA");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 200, 370, ALLEGRO_ALIGN_CENTRE , "GABRIEL FONSECA");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 520, 370, ALLEGRO_ALIGN_CENTRE , "BRUNO MOREIRA");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 800, 270, ALLEGRO_ALIGN_CENTRE , "VICTOR BRUNO");
            	al_draw_text(font_1, al_map_rgb(255, 0, 0), 100, 440, ALLEGRO_ALIGN_LEFT , "02. MONITOR");
            	al_draw_text(font_2, al_map_rgb(0,0,0), 365, 510, ALLEGRO_ALIGN_LEFT, "HEITOR SAMMUEL (HSCS)");
                al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA / 2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN TO MENU");
                    
            	al_flip_display();
                al_clear_to_color(al_map_rgb(0, 0, 0));
                FPSLimit();
            }
        }
//-------------------------------------------------------------------------------------------------------------------------------------
        char ipTemp[16] = { "" };
        char ipOficial[16] = { "0.0.0.0" };
        char nickname[16] = { "<seu_nick>" };
        char nicknameTemp[16] = { "" };
//-------------------------------------------------------------------------------------------------------------------------------------
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
                                    menuErroConnectionFailure();  // erro: nao da pra sair dessa tela *******CORRIGIR********
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
                                    //teamSelection = true;
                                    receberMapa = true;
                                    pacoteClient.tipoPacote = 0;
                                    strcpy(pacoteClient.playerName, nickname);
                                    //strcpy(EU.playerName, nickname);
                                    break;
                            }
                        }
                    } 
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
                
                al_draw_bitmap(menuNormal, 0, 0, 0);
                al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 + 20, LARGURA/2 + 320, ALTURA/2 + 70, al_map_rgb(255, 0, 0), 2);
                al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 - 100, LARGURA/2 + 320, ALTURA/2 - 50, al_map_rgb(255,0,0), 2);
                al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
                al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-10, ALTURA/2 - 150, ALLEGRO_ALIGN_CENTRE, "ENTER THE SERVER'S IP:");
                al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, ALTURA/2 - 94, ALLEGRO_ALIGN_CENTRE, ipOficial);
                al_draw_text(font_1, al_map_rgb(255, 0, 0), LARGURA/2-10, ALTURA/2 - 30, ALLEGRO_ALIGN_CENTRE, "ENTER YOUR NICKNAME:");
                al_draw_text(font_1, al_map_rgb(0,0,0), LARGURA/2, ALTURA/2 + 26, ALLEGRO_ALIGN_CENTRE, nickname);
                al_draw_text(font_2, al_map_rgb(0,0,0), LARGURA/2 + 5, ALTURA/2 + 80, ALLEGRO_ALIGN_CENTRE, "(MUST BE AT LEAST 5 CHARACTERS LONG)");
                al_draw_text(font_2, al_map_rgb(255, 0, 0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
                if(flag == 0) al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 - 95 , 0);
                else al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 + 25 , 0);
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                FPSLimit();
            }
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        sendMsgToServer(&pacoteClient, sizeof(pacoteClient));   ///PRIMEIRA MSG DO PLAYER PARA O SERVER, "OFICIALIZA" A CONEXAO E EVITA BUGS
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(teamSelection){
            bool notReady = true;
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        teamSelection = false;
                        menuConnection = true;
                        flag = 0;
                        strcpy(ipOficial, "0.0.0.0");
                        strcpy(ipTemp, "");
                        strcpy(nickname, "<seu_nick>");
                        strcpy(nicknameTemp, "");
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){

                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){

                    }
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();

                
            }

            al_draw_bitmap(menuNormal, 0, 0, 0);
            al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "TEAM LOBBY");
            al_draw_text(font_1, al_map_rgb(255,  0,  0), LARGURA/2 - 300, 110, ALLEGRO_ALIGN_CENTRE, "RED TEAM");
            al_draw_text(font_1, al_map_rgb(  0,  0,255), LARGURA/2 + 260, 110, ALLEGRO_ALIGN_CENTRE, "BLUE TEAM");
            if(notReady) al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 610, ALLEGRO_ALIGN_CENTRE, "WAITING FOR PLAYERS...");
            al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            FPSLimit();
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        while(receberMapa){
            pacoteClient.tipoPacote = 2;
            sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
            while(recvMsgFromServer(&pacoteDoServer, DONT_WAIT) == NO_MESSAGE){
                printf("esperando mapa...\n");
            }
            printf("Recebi pra entrar!\n");
            for(i=0; i<24; i++){
                for(j=0; j<32; j++){
                    map[i][j] = pacoteDoServer.mapa[i][j];
                }
            }
            for(i=0; i<6; i++) Alteracoes.olhando[3+i] = 'd';
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
                    }
                }
            }
            if(l == 0){
                if(recvMsgFromServer(&pacoteDoServer, DONT_WAIT) != NO_MESSAGE){
                    for(i=0; i<24; i++){
                        for(j=0; j<24; j++){
                            map[i][j] = pacoteDoServer.mapa[i][j];
                        }
                    }
                    l++;
                }
            }
            else{
                if(recvMsgFromServer(&Alteracoes, DONT_WAIT) != NO_MESSAGE){
                    // printf("Mudou a matriz!\n");
                    map[Alteracoes.oldx][Alteracoes.oldy] = 0;
                    map[Alteracoes.newx][Alteracoes.newy] = Alteracoes.idMoved;
                    // printf("Posicoes:\nAntiga: %d %d\nNova: %d %d\n", Alteracoes.oldx, Alteracoes.oldy, Alteracoes.newx, Alteracoes.newy);
                    // printf("Valor antigo: %d\n Valor novo: %d\n", map[Alteracoes.oldx][Alteracoes.oldy], map[Alteracoes.newx][Alteracoes.newy]);
                }
            }
                for(i=0; i<24; i++){
                    for(j=0; j<32; j++){
                            switch(map[i][j]){
                                case 0:
                                    al_draw_bitmap_region(tileSet, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                    break;
                                case 1:
                                    al_draw_bitmap_region(tileSet, 0, 0, 32, 32, TILE*j, TILE*i, 0);
                                    al_draw_bitmap_region(tileSet, 32, 32, 32, 32, TILE*j, TILE*i, 0);
                                    break;
                                case 3: //player
                                    switch(Alteracoes.olhando[3]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                                case 4: //player
                                    switch(Alteracoes.olhando[4]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                                case 5:
                                    switch(Alteracoes.olhando[5]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 9*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 6*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 7*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 8*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                                case 6:
                                    switch(Alteracoes.olhando[6]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 0*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                                case 7:
                                    switch(Alteracoes.olhando[7]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,     0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 1*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                                case 8:
                                    switch(Alteracoes.olhando[8]){
                                        case 'u':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 5*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'd':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 2*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'l':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 3*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                        case 'r':
                                            al_draw_bitmap_region(tileSet,      0,      0, 32, 32, TILE*j, TILE*i, 0);
                                            al_draw_bitmap_region(tileSet, 2*TILE, 4*TILE, 32, 32, TILE*j, TILE*i, 0);
                                            break;
                                    }
                                    break;
                            }
                    }
                }
                //drawplayer
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
void menuErroSocket()  ////VER ERRO!
{
    int vdd = 1;
    while(vdd){
        startTimer();
        while(!al_is_event_queue_empty(eventsQueue)){
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
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
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_CHAR){
                vdd = 0;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
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
            printf("cheguei\n");
            ALLEGRO_EVENT event;
            al_wait_for_event(eventsQueue, &event);
            if(event.type == ALLEGRO_EVENT_KEY_DOWN){
                vdd = false;
            }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
        }
        al_draw_bitmap(menuNormal, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2 + 10, 315, ALLEGRO_ALIGN_CENTRE, "OOPS! CONNECTION FAILURE.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2 - 20, 361, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}