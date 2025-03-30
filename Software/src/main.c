#define RODANDO_NA_PLACA 0

#define DEV_MODO 1

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


// Funcoes hardware
void readBotoes();
char botao____(char);
char botaoLeft();
char botaoUp();
char botaoDown();
char botaoRight();

void readSwitches();
int switches();
int switch_(int);

char contaBits[16];
void initContaBit();
char forcaReator();
char energiaEscudos();
char energiaArmas();
char quantasVelas();
char quantasValvulas();

int mapeiaDir(int);
int mapeiaEsq(int, int);
void writePerifericos();
void resetPerifericos();

char rd_botoes;
#if RODANDO_NA_PLACA
int fd;
#endif
void readBotoes() {
    #if RODANDO_NA_PLACA
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
    #if RODANDO_NA_PLACA
    // LÊ DOS SWICHES
    ioctl(fd, RD_SWITCHES);
    read(fd, &rd_switches, sizeof(rd_switches));
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

const int map_7_seg[10] = { // 1 is active
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};
int mapeiaDir(int bat) {
    return ~(
        (map_7_seg[bat/1000%10]<<24) +
        (map_7_seg[bat/100 %10]<<16) +
        (map_7_seg[bat/10  %10]<< 8) +
        (map_7_seg[bat     %10]    )
    );
}
int mapeiaEsq(int tem, int pre) {
    return ~(
        (map_7_seg[tem/10%10]<<24) +
        (map_7_seg[tem   %10]<<16) +
        (map_7_seg[pre/10%10]<<8) +
        (map_7_seg[pre   %10])
    );
}  
void writePerifericos() {
    #if RODANDO_NA_PLACA
    
    static int mapped;
    // ESCREVE BATERIA NO 7SEG DA DIREITA
    mapped = mapeiaDir(BATERIA);
    ioctl(fd, WR_R_DISPLAY);
    write(fd, &mapped, sizeof(mapped));
    // ESCREVE TEMP e PRESSAO NOS 7SEG DA ESQUERDA
    mapped = mapeiaEsq(TEMPERATURA, PRESSAO);
    ioctl(fd, WR_L_DISPLAY);
    write(fd, &mapped, sizeof(mapped));
    
    // ESCREVE LEITURA NOS LEDS
    ioctl(fd, WR_RED_LEDS);
    write(fd, &rd_switches, sizeof(rd_switches));
    // ANIMACAO DOS LEDS VERDES
    int verde = rand();
    ioctl(fd, WR_GREEN_LEDS);
    write(fd, &verde, sizeof(verde));
    
    #else 
    // fazer nada
    #endif
}
void resetPerifericos() {
    #if RODANDO_NA_PLACA
    
    static int reset;
    
    // RESETA LEDS
    reset = 0;
    ioctl(fd, WR_RED_LEDS);
    write(fd, &reset, sizeof(reset));
    ioctl(fd, WR_GREEN_LEDS);
    write(fd, &reset, sizeof(reset));
    
    // RESETA 7SEG DA DIREITA E DA ESQUERDA
    reset = ~0;
    ioctl(fd, WR_R_DISPLAY);
    write(fd, &reset, sizeof(reset));
    ioctl(fd, WR_L_DISPLAY);
    write(fd, &reset, sizeof(reset));
    
    #else 
    // fazer nada
    #endif
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
int BATERIA = 1000, PRESSAO = 0, TEMPERATURA = 0;
int COMBUSTIVEL = 10000, FLUIDO = 100;
int DANOS = 0;

void runMenu() {
    if(botaoRight()) {
        if(opcao==2) opcao = 0;
        else opcao++;
    }
    if(botaoLeft()) {
        if(opcao==0) opcao = 2;
        else opcao--;
    }
    
    if(botaoUp())
    	cena_atual = opcao+1,
        BATERIA = 1000, PRESSAO = 0, TEMPERATURA = 0;
}
void runControles() {
    if(botaoDown()) cena_atual = MENU;
}
void runCreditos() {
    if(botaoDown()) cena_atual = MENU;
}
void runJogo() {
    
    // slides
    if(botaoDown()) {
        cena_atual = MENU, resetPerifericos();
        return;
    }
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
    writePerifericos();
    
    // Coisas que so rodam em algumas fases
    switch (FASE) {
        case 1:
            //
        break;
        case 2:
    		BATERIA--;
            //
        break;
        case 3:
    		BATERIA--;
        	PRESSAO++;
        	TEMPERATURA++;
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
// TODO: make static or global (to precompute)
int _SwWi, _SwHe, _GapWi;

int ponto_qtd[18] = {
    4,3,3,4, 4,3,3,4, 4,3,3,4, 10,10, 4,3,3,4
};
Vector2 pontos[18][10] = {};

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

void DrawLineStripEx(const Vector2 *points, int pointCount, float thick, Color color) {
    static int i;
    for(i=1;i<pointCount;i++) 
        DrawLineEx(points[i-1], points[i], thick, color);
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
    
    #define DrawSwitchRec() \
    DrawRectangle( \
        _GapWi + (i)*(_GapWi+_SwWi), \
        ScHe - (ScHe/10), \
        _SwWi, _SwHe, \
        _cor \
    )
    #define DrawNetLines() \
    DrawLineStripEx( pontos[i], ponto_qtd[i], 3, _cor )
    static Color _cor;
    Color CORON = (Color){200, 10, 25,255}, COROFF = (Color){ 40, 40, 25,255};
    int i = 0;
    for(; i < 12; i++) {
        _cor = ((FASE!=1) && switch_(17-i)) ?  CORON : COROFF;
        // TODO: desenha 'net' do botao, fios que conectam botao a um componente
        // cor da net muda quando se aperta um botao; tipo redstone
        
        DrawNetLines();
        DrawSwitchRec();
    }
    for(; i < 18; i++) {
        _cor = ((FASE==3) && switch_(17-i)) ?  CORON : COROFF;
        
        DrawNetLines();
        DrawSwitchRec();
    }
}


// Cena separada de início do jogo
int frame = 0;
const int fade_frames = 24;
void animarfade(Color *fade) {
    fade->a -= 255/fade_frames;
    if(frame==fade_frames) fade->a = 0;
    
    DrawRectangle(0, 0, ScWi, ScHe, *fade);
}
void animartexto(char *texto) {
    static int ind = 0;
    static char temp;
    
    if(texto[ind]) ind++;
    if(botaoRight())
        for(int i = 0; i < 10 && texto[ind]; i++)
            ind++; 
    
    temp = texto[ind];
    texto[ind] = 0;
    
    DrawText(texto, 190, 200, 20, GRAY);
    
    texto[ind] = temp;
    
    frame++;
}
void jogoInicio() {
    
    Color fade = (Color){0, 0, 0, 255};
    
    char texto[] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus ut purus eget tortor dapibus sodales.\n\
Integer blandit nisi ut hendrerit gravida. Pellentesque lorem arcu, imperdiet nec justo laoreet, feugiat\n\
porta turpis. Morbi scelerisque sem vel risus ultrices, ut iaculis odio posuere.\
";
    
    SetTargetFPS(24);
    while(!WindowShouldClose() && !botaoUp() && frame <= fade_frames) {
        readBotoes();
        _BD
            ClearBackground(RAYWHITE);
            animarfade(&fade);
        _ED
        frame++;
    }
    while(!WindowShouldClose() && !botaoUp()) {
        readBotoes();
        _BD
            ClearBackground(RAYWHITE);
            animartexto(texto);
        _ED
        frame++;
    }
    SetTargetFPS(12);
}

void dev_print() {
    char texto[1024];
    snprintf(texto, 1024, 
"COMBUSTIVEL: %i\n\
BATERIA: %i\n\
PRESSAO: %i\n\
TEMPERATURA: %i\n\
FLUIDO: %i\n\
LCD:\n\
|\n\
|\n", 
    COMBUSTIVEL, BATERIA, PRESSAO, TEMPERATURA, FLUIDO);
    DrawText(texto, 10, 10, 20, BLACK);
}

void initGeral() {
    
    InitWindow(0, 0, "jogo nave");
    SetTargetFPS(12);
    
    ToggleFullscreen();
    HideCursor();
    
    int temp = GetCurrentMonitor();
    ScWi = GetMonitorWidth(temp);
    ScHe = GetMonitorHeight(temp);
    _x1 = (int)(0.26  * ((double)ScWi));
    _x2 = (int)(0.47  * ((double)ScWi));
    _y1 = (int)(0.40  * ((double)ScHe));
    _y2 = (int)(0.475 * ((double)ScHe));
    _SwWi = ScWi/40, _SwHe = ScHe/15; 
    _GapWi = (ScWi-_SwWi*18)/19;
    
    // TODO: estabelecer pontos de referencia (em funcao de ScWi ou ScHe)
    int Y0 = 0, YFa = 0, YFb = 0;
    Vector2 pontos_ref[18][10] = { 
 
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    
    {{}, {0, Y0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, YFb}},
    {{}, {0, Y0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, YFb}},
    
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, YFa}},
    {{}, {0, Y0}, {0, Y0 }, {0, YFa}}
    
    };
    
    // pontos base
    for(int i = 0; i < 18; i++)
        pontos[i][0] = (Vector2){_GapWi + (i)*(_GapWi+_SwWi) + _SwWi/2, ScHe - (ScHe/10)};
    // pontos da referencia
    for(int i = 0; i < 18; i++)
        for(int j = 1; j < 10; j++)
            pontos[i][j] = pontos_ref[i][j];
    // possiveis pontos computaveis
    for(int i = 0; i < 18; i++)
        pontos[i][3] = (Vector2){pontos[i][0].x, ScHe - (ScHe/7)};
    
    initFundos();
    
    initContaBit();
    
    #if RODANDO_NA_PLACA
    fd = open(argv[1], O_RDWR);
    #endif
    
    /* TODO: cria THREAD para leitura de switches, botões e escrita de leds.. */
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
    initGeral();
    //--------------------------------------------------------------------------------------
    
    // Cutscene inicial do jogo
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
        #if DEV_MODO
        dev_print();
        #endif
        _ED
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    jogo_fechado = 1;
    
    destroiFundos();
    
    CloseWindow();        // Close window and OpenGL context
    
    #if RODANDO_NA_PLACA
    close(fd);
    #endif
    //--------------------------------------------------------------------------------------

    return 0;
}