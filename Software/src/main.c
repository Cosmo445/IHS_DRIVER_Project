#define RODANDO_NA_PLACA 0

#if RODANDO_NA_PLACA

#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, atoi, rand... */
#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */

// ioctl commands defined for the pci driver header
#include "../include/ioctl_cmds.h"

#endif

#include <stdio.h>
#include "raylib.h"

//https://www.raylib.com/cheatsheet/cheatsheet.html
//https://github.com/raysan5/raylib/discussions/2478


// Funcoes hardware (((TODO: fazer)))
void readBotoes();
char botao____(char);
char botaoLeft();
char botaoUp();
char botaoDown();
char botaoRight();

void readSwitches();
int switches();
int switch_(int);

void leds_verm(int);
void leds_verd(int);
char contaBits[16];
void initContaBit();
char forcaReator();
char energiaEscudos();
char energiaArmas();
char quantasVelas();
char quantasValvulas();

char rd_botoes;
#ifdef O_RDWR
int fd;
#endif
void readBotoes() {
    #ifdef RD_PBUTTONS
    // LÊ DOS BOTOES DA PLACA
    ioctl(fd, RD_PBUTTONS);
    read(fd, &rd_botoes, 1);
    #else
    rd_botoes = 
        (IsKeyPressed(KEY_LEFT)  <<3) +
        (IsKeyPressed(KEY_UP)    <<2) +
        (IsKeyPressed(KEY_DOWN)  <<1) +
        (IsKeyPressed(KEY_RIGHT) <<0)
    ;
    #endif
}
char botao____(char qual) {
    static char retorno = 0, pressed_prev[4] = {1,1,1,1}, pressed_now = 0;
    pressed_now = !(rd_botoes&(1<<qual));
	retorno = (!pressed_prev[qual]) && pressed_now; // se pressionado, mas n acabou de pressionar
	pressed_prev[qual] = pressed_now;             // guarda pressed now pra proxima chamada
	return retorno;
}
char botaoLeft() { 
    return botao____(3);
}
char botaoUp() { 
    return botao____(2);
}
char botaoDown() {
    return botao____(1);
}
char botaoRight() { 
    return botao____(0);
}

int rd_switches;
void readSwitches() {
    #ifdef RD_SWITCHES
    ioctl(fd, RD_SWITCHES);
    read(fd, &rd_switches, 4);
    #else
    static int pressed_prev = 0, pressed_now = 0, pressed_true;
    pressed_now =
        (((int)IsKeyPressed(KEY_E))<<17) +
        (((int)IsKeyPressed(KEY_R))<<16) +
        (((int)IsKeyPressed(KEY_T))<<15) +
        (((int)IsKeyPressed(KEY_Y))<<14) +
        (((int)IsKeyPressed(KEY_U))<<13) +
        (((int)IsKeyPressed(KEY_I))<<12) +
        (((int)IsKeyPressed(KEY_O))<<11) +
        (((int)IsKeyPressed(KEY_P))<<10) +
        (((int)IsKeyPressed(KEY_H))<< 9) +
        (((int)IsKeyPressed(KEY_J))<< 8) +
        (((int)IsKeyPressed(KEY_K))<< 7) +
        (((int)IsKeyPressed(KEY_L))<< 6) +
        (((int)IsKeyPressed(KEY_X))<< 5) +
        (((int)IsKeyPressed(KEY_C))<< 4) +
        (((int)IsKeyPressed(KEY_V))<< 3) +
        (((int)IsKeyPressed(KEY_B))<< 2) +
        (((int)IsKeyPressed(KEY_N))<< 1) +
        (((int)IsKeyPressed(KEY_M))<< 0)
    ;
    pressed_true = (~pressed_prev) & pressed_now;
    pressed_prev = pressed_now;
    rd_switches ^= pressed_true;
    #endif
}
int switches() {
    return rd_switches;
}
int switch_(int qual){
    return rd_switches&(1<<qual);
}

void leds_verm(int valor) {
    ;
}
void leds_verd(int valor) {
    ;
}

void initContaBit() {
    // TODO implementar em assembly
    for(int i = 0; i < 16; i++) {
        contaBits[i]=0;
        for(int j = i; j; j>>=1)
            contaBits[i]+=j&1;
    }
}
char forcaReator() {
    return contaBits[(switches()>>14)&15];
}
char energiaEscudos() {
    return contaBits[(switches()>>10)&15];
}
char energiaArmas() {
    return contaBits[(switches()>> 6)&15];
}
char quantasVelas() {
    return contaBits[(switches()>> 4)& 3];
}
char quantasValvulas() {
    return contaBits[(switches()>> 0)&15];
}


// Cenas do jogo
enum cenas_enum {
    MENU = 0,
    CONTROLES = 1,
    CREDITOS = 2,
    JOGO = 3,
};

int cena_atual = MENU;

int FASE = 1;


// Update de variáveis
void runMenu();
void runControles();
void runCreditos();
void runJogo();

enum opcoes_menu {
    op_CONTROLES = 0,
    op_CREDITOS = 1,
    op_JOGO = 2
};
int opcao = 0;
    
void runMenu() {
    if(botaoRight()) {
        if(opcao==2) opcao = 0;
        else opcao++;
    }
    if(botaoLeft()) {
        if(opcao==0) opcao = 2;
        else opcao--;
    }
    
    if(botaoUp()) cena_atual = opcao+1;
}
void runControles() {
    if(botaoDown()) cena_atual = MENU;
}
void runCreditos() {
    if(botaoDown()) cena_atual = MENU;
}
void runJogo() {
    
    // slides
    if(botaoDown()) cena_atual = MENU;
    if(botaoRight()) {
        if(FASE<3) FASE++;
        else FASE = 1;
    }
    if(botaoLeft()) {
        if(FASE>1) FASE--;
        else FASE = 3;
    }
    
    // Coisas que rodam em todas a fases
    readSwitches();
    
    // Coisas que so rodam em algumas fases
    switch (FASE) {
        case 1:
            //
        break;
        case 2:
            //
        break;
        case 3:
            //
    }
}


// Interface gráfica
void telaMenu();
void telaControles();
void telaCreditos();
void telaJogo();

void telaInicio();

Texture2D 
    FundoMenu,
    FundoControles,
    FundoCreditos,
    FundoJogo_1,
    FundoJogo_2,
    FundoJogo_3
;

int ScWi, ScHe;
int _x1, _x2, _y1, _y2; //coordenadas para print no menu

// TODO: fundos menu, controles, creditos
void initFundos() {
    Image Fundo_img_temp;
    
    Fundo_img_temp = LoadImage("./fundo/Jogo_1.png");
    ImageResize(&Fundo_img_temp, ScWi, ScHe);
        FundoJogo_1 = LoadTextureFromImage(Fundo_img_temp); 
    UnloadImage(Fundo_img_temp);
    
    Fundo_img_temp = LoadImage("./fundo/Jogo_2.png");
    ImageResize(&Fundo_img_temp, ScWi, ScHe);
        FundoJogo_2 = LoadTextureFromImage(Fundo_img_temp); 
    UnloadImage(Fundo_img_temp);
    
    Fundo_img_temp = LoadImage("./fundo/Jogo_3.png");
    ImageResize(&Fundo_img_temp, ScWi, ScHe);
        FundoJogo_3 = LoadTextureFromImage(Fundo_img_temp); 
    UnloadImage(Fundo_img_temp);
    
    Fundo_img_temp = LoadImage("./fundo/Menu.png");
    ImageResize(&Fundo_img_temp, ScWi, ScHe);
        FundoMenu = LoadTextureFromImage(Fundo_img_temp); 
    UnloadImage(Fundo_img_temp);
    
    //Fundo = LoadTexture("./texturas/Fundo.png");
}

void destroiFundos(){
    UnloadTexture(FundoJogo_1);
    UnloadTexture(FundoJogo_2);
    UnloadTexture(FundoJogo_3);
    UnloadTexture(FundoMenu);
}

#define _BD BeginDrawing();
#define _ED EndDrawing();
void telaMenu() {
    ClearBackground(RAYWHITE);
    
    DrawText("MENU", 190, 200, 20, GRAY);
    
    DrawTexture(FundoMenu, 0, 0, WHITE);
    
    switch (opcao) {
        case op_CONTROLES:
            DrawText(">", _x1, _y1, 60, BLACK);
        break;
        case op_CREDITOS:
            DrawText(">", _x2, _y1, 60, BLACK);
        break;
        case op_JOGO:
            DrawText(">", _x1, _y2, 60, BLACK);
    }
}
void telaControles() {
    ClearBackground(RAYWHITE);
    
    DrawText("CONTROLES", 190, 200, 20, GRAY);
}
void telaCreditos() {
    ClearBackground(RAYWHITE);
    
    DrawText("CREDITOS", 190, 200, 20, GRAY);
}
void telaJogo() {
    ClearBackground(RAYWHITE);
    
    // Elementos visuais de certas fases
    switch (FASE) {
        case 1:
            DrawText("JOGO_1", 190, 200, 20, GRAY);
            DrawTexture(FundoJogo_1, 0, 0, WHITE);
        break;
        case 2:
            DrawText("JOGO_2", 190, 200, 20, GRAY);
            DrawTexture(FundoJogo_2, 0, 0, WHITE);
        break;
        case 3:
            DrawText("JOGO_3", 190, 200, 20, GRAY);
            DrawTexture(FundoJogo_3, 0, 0, WHITE);
    }
    
    // Elementos visuais de todas a fases
    
    // TODO: make static or global (to precompute)
    int SwWi = ScWi/40, SwHe=ScHe/15, GapWi = (ScWi-SwWi*18)/19;
    for(int i = 0; i < 18; i++) {
        
        // TODO: desenha 'net' do botao, fios que conectam botao a um componente
        // cor da net muda quando se aperta um botao; tipo redstone
        
        DrawRectangle(
            GapWi+(i)*(GapWi+SwWi), 
            ScHe-(ScHe/10), 
            SwWi, SwHe, 
            switch_(17-i) ? (Color){200,10,10,255} : (Color){10,10,10,255}
        );
    }
}


// Cena separada de início do jogo
int frame = 0;
int fade_frames = 24;
void animarfade(Color *fade) {
    if(frame<=fade_frames) {
        
        fade->a -= 255/fade_frames;
        if(frame==fade_frames) fade->a = 0;
        
        DrawRectangle(0, 0, ScWi, ScHe, *fade);
        
        frame++;
    }
}
void animartexto(char *texto) {
    static int ind = 0;
    static char temp;
    
    if(frame>fade_frames) {
        if(texto[ind]) ind++;
        
        temp = texto[ind];
        texto[ind] = 0;
        
        DrawText(texto, 190, 200, 20, GRAY);
        
        texto[ind] = temp;
        
        frame++;
    }
}
void jogoInicio() {
    
    Color fade = (Color){0, 0, 0, 255};
    
    char texto[] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut purus eget tortor dapibus sodales.\n\
Integer blandit nisi ut hendrerit gravida. Pellentesque lorem arcu, imperdiet nec justo laoreet, feugiat\n\
porta turpis. Morbi scelerisque sem vel risus ultrices, ut iaculis odio posuere.\
";
    
    SetTargetFPS(24);
    while(!WindowShouldClose() && !botaoUp()) {
        
        readBotoes();
        
        _BD
            ClearBackground(RAYWHITE);
            
            animarfade(&fade);
            
            animartexto(texto);
            
        _ED
    }
    SetTargetFPS(12);
}


//variavel global lida por cada thread para verificar se o jogo foi fechado
int jogo_fechado = 0; 

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(0, 0, "jogo nave");
    SetTargetFPS(12);
    
    ToggleFullscreen();
    HideCursor();
    
    int temp = GetCurrentMonitor();
    ScWi = GetMonitorWidth(temp);
    ScHe = GetMonitorHeight(temp);
        _x1 = (int)(0.26  * ((double)ScWi)),
        _x2 = (int)(0.47  * ((double)ScWi)),
        _y1 = (int)(0.40  * ((double)ScHe)),
        _y2 = (int)(0.475 * ((double)ScHe))
    ;
    
    #ifdef O_RDWR
    fd = open(argv[1], O_RDWR);
    #endif
    
    initFundos();
    
    initContaBit();
    
    /* TODO: cria THREAD para leitura de switches, botões e escrita de leds.. */
    
    //--------------------------------------------------------------------------------------
    
    jogoInicio();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        readBotoes();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Estados de jogo
        _BD
        switch (cena_atual) {
            case MENU:
                runMenu();
                telaMenu();
            break; 
            case CONTROLES:
                runControles();
                telaControles();
            break; 
            case CREDITOS:
                runCreditos();
                telaCreditos();
            break; 
            case JOGO:
                runJogo();
                telaJogo();
        }
        _ED
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    jogo_fechado = 1;
    
    destroiFundos();
    
    CloseWindow();        // Close window and OpenGL context
    
    #ifdef O_RDWR
    close(fd);
    #endif
    //--------------------------------------------------------------------------------------

    return 0;
}