#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"

#define LARGURA 1024
#define ALTURA 768

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
    int count = 0;
    int option = 0;
    int conecta = 0;
    int flag = 0;

    Player pacoteClient;
    
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
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) inicio = false;
        }
		al_draw_bitmap(menuImg, 0, 0, 0);
        al_draw_textf(font_1, al_map_rgb(255,255,255), LARGURA/2 - 140, 50, 0, "TARGET");
        if(Blink() == 1) 
            pisca = !pisca;

        if(pisca)
            al_draw_textf(font_1, al_map_rgb(255,0,0), LARGURA/2 - 335, ALTURA/2 + 40, 0, "PRESS ANY KEY TO BEGIN");

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
		        }
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();   // APERTOU O 'X' DA JANELA 
            al_draw_bitmap(menuImg, 0, 0, 0);
            al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE , "TARGET");
            al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) - 100, ALLEGRO_ALIGN_CENTRE , "START GAME");
            al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2), ALLEGRO_ALIGN_CENTRE , "HELP");
            al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) + 100, ALLEGRO_ALIGN_CENTRE , "CREDITS");
            al_draw_text(font_1, al_map_rgb(255,0,0), (LARGURA/2), (ALTURA/2) + 200, ALLEGRO_ALIGN_CENTRE , "EXIT GAME");
            switch(option){
                case 0:
                    al_draw_bitmap(seta, LARGURA/2 - 250, ALTURA/2 - 110, 0);
                    al_draw_bitmap(seta, LARGURA/2 + 170, ALTURA/2 - 110, -1);
                    break;
                case 1:
                    al_draw_bitmap(seta, LARGURA/2 - 250, ALTURA/2 - 10, 0);
                    al_draw_bitmap(seta, LARGURA/2 + 170, ALTURA/2 - 10, -1);
                    break;
                case 2:
                    al_draw_bitmap(seta, LARGURA/2 - 250, ALTURA/2 + 90, 0);
                    al_draw_bitmap(seta, LARGURA/2 + 170, ALTURA/2 + 90, -1);
                    break;
                case 3:
                    al_draw_bitmap(seta, LARGURA/2 - 250, ALTURA/2 + 190 , 0);
                    al_draw_bitmap(seta, LARGURA/2 + 170, ALTURA/2 + 190 , -1);
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

        al_draw_bitmap(menuImg, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 50, ALLEGRO_ALIGN_CENTRE , "HELP");
        al_draw_text(font_1, al_map_rgb(255,255,255), 100 , 120, ALLEGRO_ALIGN_LEFT , "01. MOVES");
        al_draw_text(font_2, al_map_rgb(255,255,255), 200, 200, ALLEGRO_ALIGN_CENTRE , "UP");
        al_draw_text(font_2, al_map_rgb(255,255,255), 600, 200, ALLEGRO_ALIGN_CENTRE , "DOWN");
        al_draw_text(font_2, al_map_rgb(255,255,255), 200, 270, ALLEGRO_ALIGN_CENTRE , "LEFT");
        al_draw_text(font_2, al_map_rgb(255,255,255), 600, 270, ALLEGRO_ALIGN_CENTRE , "RIGHT");
        al_draw_text(font_2, al_map_rgb(255,255,255), 200, 340, ALLEGRO_ALIGN_CENTRE , "SPACE BAR");
        al_draw_text(font_1, al_map_rgb(255,255,255), 100, 420, ALLEGRO_ALIGN_LEFT, "02. OBJECTIVE");
        al_draw_text(font_2, al_map_rgb(255,255,255), 200, 500, ALLEGRO_ALIGN_LEFT, "YOU MUST DEFEAT THE ENEMY TARGET");
        al_draw_text(font_2, al_map_rgb(255,255,255), 200, 540,  ALLEGRO_ALIGN_LEFT, "WHILE PROTECTING YOURS");
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
              	al_draw_bitmap(menuImg, 0, 0, 0);
            	al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA / 2 , 20,ALLEGRO_ALIGN_CENTRE , "CREDITS");
            	al_draw_text(font_1, al_map_rgb(255,255,255), 100, 200, ALLEGRO_ALIGN_LEFT , "01. STUDENTS");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 200, 270, ALLEGRO_ALIGN_CENTRE , "RAFAEL VASCONCELOS");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 500, 270, ALLEGRO_ALIGN_CENTRE , "GABRIEL GOMES");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 200, 320, ALLEGRO_ALIGN_CENTRE , "MATHEUS CARACCIOLO");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 500, 320, ALLEGRO_ALIGN_CENTRE , "ARTHUR BARA");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 200, 370, ALLEGRO_ALIGN_CENTRE , "GABRIEL FONSECA");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 500, 370, ALLEGRO_ALIGN_CENTRE , "BRUNO");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 800, 270, ALLEGRO_ALIGN_CENTRE , "VITINHO");
            	al_draw_text(font_1, al_map_rgb(255,255,255), 100, 440, ALLEGRO_ALIGN_LEFT , "02. MONITOR");
            	al_draw_text(font_2, al_map_rgb(255,255,255), 200, 510, ALLEGRO_ALIGN_LEFT, "HEITOR MITO GOD HOMAO DA PORRA");
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
                    if(flag == 0){
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
                    } else{
                        if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                            menuConnection = false;
                            menu = true;
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
                                    break;
                                case -1:
                                    menuErroConnectionFailure();
                                    break;
                                case 0:
                                    menuErroServerFull();
                                    break;
                                case 1:
                                    menuConnection = false;
                                    teamSelection = true;
                                    pacoteClient.tipoPacote = 0;
                                    strcpy(pacoteClient.playerName, nickname);
                                    break;
                            }
                        }
                    } 
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
                
                al_draw_bitmap(menuImg, 0, 0, 0);
                al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
                al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-10, ALTURA/2 - 150, ALLEGRO_ALIGN_CENTRE, "ENTER THE SERVER'S IP:");
                al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 - 100, LARGURA/2 + 320, ALTURA/2 - 50, al_map_rgb(255,0,0), 2);
                al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, ALTURA/2 - 94, ALLEGRO_ALIGN_CENTRE, ipOficial);
                al_draw_text(font_1, al_map_rgb(255, 0, 0), LARGURA/2-10, ALTURA/2 - 30, ALLEGRO_ALIGN_CENTRE, "ENTER YOUR NICKNAME:");
                al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, ALTURA/2 + 26, ALLEGRO_ALIGN_CENTRE, nickname);
                al_draw_rectangle(LARGURA/2 - 320, ALTURA/2 + 20, LARGURA/2 + 320, ALTURA/2 + 70, al_map_rgb(255, 0, 0), 2);
                al_draw_text(font_2, al_map_rgb(255, 0, 0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
                if(flag == 0) al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 - 100 , 0);
                else al_draw_bitmap(seta, LARGURA/2 - 405, ALTURA/2 + 20 , 0);
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                FPSLimit();
            }
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
        sendMsgToServer(&pacoteClient, sizeof(pacoteClient));
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
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){

                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){

                    }
                }
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();

                
            }

            al_draw_bitmap(menuImg, 0, 0, 0);
            al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "TEAM LOBBY");
            if(notReady) al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 610, ALLEGRO_ALIGN_CENTRE, "WAITING FOR PLAYERS...");
            al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2, 640, ALLEGRO_ALIGN_CENTRE, "PRESS ESC TO RETURN TO MENU");
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            FPSLimit();
        }
//------------------------------------------------------------------------------------------------------------------------------------------------------
       /* while(gameOn){
            startTimer();
            while(!al_is_event_queue_empty(eventsQueue)){
                ALLEGRO_EVENT event;
                al_wait_for_event(eventsQueue, &event);
                if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) allegroEnd();
            }
        }*/
    }
    return 0;       
}

void menuErroSocket()
{
    int vdd = 1;
    eventsQueue = al_create_event_queue();
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
        al_draw_bitmap(menuImg, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-100, 384, ALLEGRO_ALIGN_CENTRE, "OOPS! SOCKET ERROR.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2-100, 394, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}
void menuErroServerFull()
{
    int vdd = 1;
    eventsQueue = al_create_event_queue();
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
        al_draw_bitmap(menuImg, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-100, 384, ALLEGRO_ALIGN_CENTRE, "OOPS! SERVER IS FULL.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2-100, 394, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}
void menuErroConnectionFailure()
{
    int vdd = 1;
    eventsQueue = al_create_event_queue();
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
        al_draw_bitmap(menuImg, 0, 0, 0);
        al_draw_text(font_1, al_map_rgb(255,255,255), LARGURA/2, 20, ALLEGRO_ALIGN_CENTRE, "CONNECTION");
        al_draw_text(font_1, al_map_rgb(255,0,0), LARGURA/2-100, 384, ALLEGRO_ALIGN_CENTRE, "OOPS! CONNECTION FAILURE.");
        al_draw_text(font_2, al_map_rgb(255,0,0), LARGURA/2-100, 384, ALLEGRO_ALIGN_CENTRE, "PRESS ANY KEY TO RETURN AND TRY AGAIN.");
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
        FPSLimit();
    }
}