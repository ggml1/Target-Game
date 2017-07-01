#include "AllegroCore.h"

bool coreInit()
{
    //modules and add-ons initialization
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    if (!al_init_font_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_font.\n");
        return false;
    }

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }


    eventsQueue = al_create_event_queue();
    if (!eventsQueue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }

 	

    return true;
}

bool windowInit(int W, int H, char title[])
{
    //window and event queue creation
    main_window = al_create_display(W, H);
    if (!main_window)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(main_window, title);

    //registra janela na fila de eventos
    al_register_event_source(eventsQueue, al_get_display_event_source(main_window));

    return true;
}

bool inputInit()
{
	/*------------------------------MOUSE--------------------------------*/
	//Inicializa Mouse
	if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(main_window);
        return false;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(main_window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(main_window);
        return false;
    }
    /*------------------------------MOUSE--------------------------------*/

    /*------------------------------TECLADO------------------------------*/
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    /*------------------------------TECLADO------------------------------*/


    //Registra mouse e teclado na fila de eventos
    al_register_event_source(eventsQueue, al_get_mouse_event_source());
    al_register_event_source(eventsQueue, al_get_keyboard_event_source());

    return true;
}

bool fontInit() // O NOME DA FONTE EH "font_1"
{
    /*------------------------------FONTE--------------------------------*/
    font_1 = al_load_font("Resources/Fonts/Boxy-Bold.ttf", 45, 0);
    if (!font_1)
    {
        fprintf(stderr, "Falha ao carregar \"Boxy-Bold.ttf\".\n");
        return false;
    }
	font_2 = al_load_font("Resources/Fonts/Boxy-Bold.ttf", 25, 0);
    if (!font_2)
    {
        fprintf(stderr, "Falha ao carregar \"Boxy-Bold.ttf\".\n");
        return false;
    }
    font_3 = al_load_font("Resources/Fonts/Boxy-Bold.ttf", 120, 0);
    if (!font_3)
    {
        fprintf(stderr, "Falha ao carregar \"Boxy-Bold.ttf\".\n");
        return false;
    }

    return true;
}

void allegroEnd()
{
    al_destroy_display(main_window);
    al_destroy_event_queue(eventsQueue);
}

void startTimer()
{
    startingTime = al_get_time();
}

void startTimerBlink()
{
    startingTimeBlink = al_get_time();
}

double getTimer()
{
    return al_get_time() - startingTime;
}

double getTimerBlink()
{
    return al_get_time() - startingTimeBlink;
}

void FPSLimit()
{
	if (getTimer() < 1.0/FPS)
    {
        al_rest((1.0 / FPS) - getTimer());
    }
}

bool Blink(){
    if(getTimerBlink() < 1) return 0;
    else{
        startTimerBlink();
        return 1;
    }
}

bool loadGraphics() // INICIA AS IMAGENS "1_bitmap", "2_bitmap", "3_bitmap", "4_bitmap" 
{
    bit1 = al_load_bitmap("Resources/Etc/1_bitmap.png");
    if (!bit1){
        fprintf(stderr, "Falha carregando 1_bitmap.png\n");
        return false;
    }
   
    bit2 = al_load_bitmap("Resources/Etc/2_bitmap.png");
    if (!bit2){
        fprintf(stderr, "Falha carregando 2_bitmap.png\n");
        return false;
    }
    
    bit3 = al_load_bitmap("Resources/Etc/3_bitmap.png");
    if (!bit3){
        fprintf(stderr, "Falha carregando 3_bitmap.png\n");
        return false;
    }
    
    bit4 = al_load_bitmap("Resources/Etc/4_bitmap.png");
    if (!bit4){
        fprintf(stderr, "Falha carregando 4_bitmap.png\n");
        return false;
    }

    tileSet = al_load_bitmap("Resources/Tilesets/tileset.png");
    if(!tileSet){
        fprintf(stderr, "Falha ao carregar o tileset.\n");
        return false;
    }

    menuOpcoes = al_load_bitmap("Resources/Etc/menuOpcoes.png");
    if(!menuOpcoes){
        fprintf(stderr, "Falha ao carregar o tileset.\n");
        return false;
    }

    menuInicio = al_load_bitmap("Resources/Etc/menuInicio.png");
    if(!menuInicio){
        fprintf(stderr, "Falha ao carregar o tileset.\n");
        return false;
    }

    menuNormal = al_load_bitmap("Resources/Etc/menuNormal.png");
    if(!menuNormal){
        fprintf(stderr, "Falha ao carregar o tileset.\n");
        return false;
    }

    seta = al_load_bitmap("Resources/Etc/seta.png");
    if(!seta){
        fprintf(stderr, "Falha ao carregar o seta.\n");
        return false;
    }

    upKey = al_load_bitmap("Resources/Etc/up.png");
    if(!upKey){
        fprintf(stderr, "Falha ao carregar a sprite UPKEY.\n");
        return false;
    }

    downKey = al_load_bitmap("Resources/Etc/down.png");
    if(!downKey){
        fprintf(stderr, "Falha ao carregar a sprite DOWNKEY.\n");
        return false;
    }

    rightKey = al_load_bitmap("Resources/Etc/right.png");
    if(!rightKey){
        fprintf(stderr, "Falha ao carregar a sprite RIGHTKEY.\n");
        return false;
    }

    leftKey = al_load_bitmap("Resources/Etc/left.png");
    if(!leftKey){
        fprintf(stderr, "Falha ao carregar a sprite LEFTKEY.\n");
        return false;
    }
    
    Dungeon_B = al_load_bitmap("Resources/Tilesets/Dungeon_B.png");
    if(!Dungeon_B){
        fprintf(stderr, "Falha ao carregar a Dungeon_B.\n");
        return false;
    }

    Outside_B = al_load_bitmap("Resources/Tilesets/Outside_B.png");
    if(!Outside_B){
        fprintf(stderr, "Falha ao carregar a Outside_B.\n");
        return false;
    }

    World_B = al_load_bitmap("Resources/Tilesets/World_B.png");
    if(!World_B){
        fprintf(stderr, "Falha ao carregar a World_B.\n");
        return false;
    }

    Dungeon_A2 = al_load_bitmap("Resources/Tilesets/Dungeon_A2.png");
    if(!Dungeon_A2){
        fprintf(stderr, "Falha ao carregar a Dungeon_A2.\n");
        return false;
    }
    
    Inside_C = al_load_bitmap("Resources/Tilesets/Inside_C.png");
    if(!Inside_C){
        fprintf(stderr, "Falha ao carregar a Inside_C.\n");
        return false;
    }
    return true;
}
