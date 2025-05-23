#include "liquid_crystal.h"

//===================================================================================
/* Recebe uma string a ser "escrita" no lcd - função mais próxima do usuário */
void lcd_write_msg(int fd, char *msg) {

    /* Vai ficar em loop até achar um '\0' */
    uint8_t count = 0;
    while (msg[count] != 0 && count < 17) {
	/* Passa o fd e a letra a ser "escrita" no lcd */
        lcd_write_char(fd, msg[count]);
        count++;
    }

    /* Se o usuário digitar uma mensagem maior que 16 letras (sem levar em conta ao '\0' final da string) */
    if(count > 16) {
        printf("Erro: overflow mensage!");
    }

}

//===================================================================================
/* "Escreve" no lcd caracteres, seguindo a tabela ASCII, rs = 1 */
void lcd_write_char(int fd, char symbol) {

    uint32_t data = 0;
    /* atualiza o valor do data[7:0] e das variáveis de controle */
    data = update_data(symbol, 1);
    /* realiza o pulso do enable para escrever no lcd */
    pulse_enable(fd, data);

}

//===================================================================================
/* "Escreve" no lcd comandos de controle, rs = 0 */
void lcd_write_control(int fd, char symbol) {

    uint32_t data = 0;
    /* atualiza o valor do data[7:0] e das variáveis de controle */
	data = update_data(symbol, 0);
    /* realiza o pico do enable para escrever no lcd */
    pulse_enable(fd, data);

}

//===================================================================================
/* "Escreve" no lcd com auxílio do drive implementado */
void lcd_drive_write(int fd, uint32_t data) {

    /* o lcd precisa de um tempo para processar as informações, devido a isso é
       necessário algum mecamismo para dar a ele esse "tempo necessário" */
    int x = 0;
    for (int i = 0; i < 50000; i++) {
	x++;
    }
	
    /* inicia a comunicação com o lcd */
    ioctl(fd, WR_LCD);
    /* escreve no ldc o conteúdo de data */
    write(fd, &data, 4);

}

//===================================================================================
/* Inicializa o lcd */
void lcd_init(int fd) {

    /* inicializa/seta as duas linhas do lcd */
    char symbol = 0x38;
    lcd_write_control(fd, symbol);
    /* ativa o cursor */
    symbol = 0x0e;
    lcd_write_control(fd, symbol);
    /* limpa o display */
    symbol = 0x01;
    lcd_write_control(fd, symbol);
    /* home */
    symbol = 0x02;
    lcd_write_control(fd, symbol);
    /* home home */
    symbol = 0x06;
    lcd_write_control(fd, symbol);

}

//===================================================================================
/* Limpa o display */
void lcd_clear(int fd) {

    char symbol = 0x01;
    lcd_write_control(fd, symbol);

}

//===================================================================================
