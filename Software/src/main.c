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
#include "ioctl_cmds.h"

#include "raylib.h"
#include <stdio.h>

//https://www.raylib.com/cheatsheet/cheatsheet.html
//https://github.com/raysan5/raylib/discussions/2478


// Funcoes hardware (((TODO: fazer)))
char botaoLeft();
char botaoUp();
char botaoDown();
char botaoRight();
int switches();
void leds_verm(int);
void leds_verd(int);
char contaBits[16];
void initContaBit();
char forcaReator();
char energiaEscudos();
char energiaArmas();
char quantasVelas();
char quantasValvulas();

char botaoLeft() 
    { return IsKeyPressed(KEY_LEFT); }
char botaoUp() 
    { return IsKeyPressed(KEY_UP); }
char botaoDown() 
    { return IsKeyPressed(KEY_DOWN); }
char botaoRight() 
    { return IsKeyPressed(KEY_RIGHT); }
int switches() {
    return 
        (((int)IsKeyPressed(KEY_V))<<17) +
        (((int)IsKeyPressed(KEY_B))<<16) +
        (((int)IsKeyPressed(KEY_N))<<15) +
        (((int)IsKeyPressed(KEY_M))<<14) +
        (((int)IsKeyPressed(KEY_E))<<13) +
        (((int)IsKeyPressed(KEY_R))<<12) +
        (((int)IsKeyPressed(KEY_T))<<11) +
        (((int)IsKeyPressed(KEY_Y))<<10) +
        (((int)IsKeyPressed(KEY_U))<< 9) +
        (((int)IsKeyPressed(KEY_I))<< 8) +
        (((int)IsKeyPressed(KEY_O))<< 7) +
        (((int)IsKeyPressed(KEY_P))<< 6) +
        (((int)IsKeyPressed(KEY_F))<< 5) +
        (((int)IsKeyPressed(KEY_G))<< 4) +
        (((int)IsKeyPressed(KEY_H))<< 3) +
        (((int)IsKeyPressed(KEY_J))<< 2) +
        (((int)IsKeyPressed(KEY_K))<< 1) +
        (((int)IsKeyPressed(KEY_L))<< 0)
    ;
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
    if(botaoDown())
        cena_atual = MENU;
}
void runCreditos() {
    if(botaoDown())
        cena_atual = MENU;
}
void runJogo() {
    if(botaoDown())
        cena_atual = MENU;
    
    if(botaoRight()) {
        if(FASE<3) FASE++;
        else FASE = 1;
    }
    if(botaoLeft()) {
        if(FASE>1) FASE--;
        else FASE = 3;
    }
    
    // Coisas que rodam em todas a fases
    
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
void telaJogo_1();
void telaJogo_2();
void telaJogo_3();

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
    _BD
        ClearBackground(BLACK);
        DrawRectangle(0, 0, ScWi, ScHe, fade);
    _ED
    
    SetTargetFPS(24);
    while(!WindowShouldClose() && !IsKeyPressed(KEY_S)) {
        
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
int main(void)
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
    
    initFundos();
    
    initContaBit();
    
    /* TODO: cria THREAD para leitura de switches. */
    
    //--------------------------------------------------------------------------------------
    
    jogoInicio();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
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
    
    CloseWindow();        // Close window and OpenGL context
    
    destroiFundos();
    //--------------------------------------------------------------------------------------

    return 0;
}