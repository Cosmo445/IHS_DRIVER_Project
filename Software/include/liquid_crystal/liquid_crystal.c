#include "liquid_crystal.h"
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include "../ioctl_cmds.h"

//===================================================================================
/* recebe uma string a ser "escrita" no lcd, função mais próxima do usuário */
void lcd_write_msg(int fd, char *msg) {

    /* vai ficar em loop até achar um '\0' */
	uint8_t count = 0;
    do {
        lcd_write_char(fd, msg[count]);
        count++;
    } while (msg[count] != 0);

}

//===================================================================================
/* "Escreve" no lcd caracteres, seguindo a tabela ASCII, rs = 1 */
void lcd_write_char(int fd, char symbol) {

    uint32_t data;
    /* atualiza o valor do data[7:0] e do rs */
	data = updade_data(symbol, 1);
    /* realiza o pico do enable para escrever no lcd */
    pulse_enable(fd, data);

}

//===================================================================================
/* "Escreve" no lcd comandos de controle, rs = 0 */
void lcd_write_control(int fd, char symbol) {

    uint32_t data;
    /* atualiza o valor do data[7:0] e do rs */
	data = updade_data(symbol, 0);
    /* realiza o pico do enable para escrever no lcd */
    pulse_enable(fd, data);

}

//===================================================================================
/* "Escreve" no lcd com auxílio do drive implementado */
void lcd_drive_write(int fd, uint32_t data) {

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
    lcd_control(fd, symbol);
    /* ativa o cursor */
    symbol = 0x0e;
    lcd_control(fd, symbol);
    /* limpa o display */
    symbol = 0x01;
    lcd_control(fd, symbol);
    /* home */
    symbol = 0x02;
    lcd_control(fd, symbol);
    /* home home */
    symbol = 0x06;
    lcd_control(fd, symbol);

}

//===================================================================================
/* Limpa o display */
void lcd_clear(int fd) {

    char symbol = 0x01;
    lcd_control(fd, symbol);

}

//===================================================================================
