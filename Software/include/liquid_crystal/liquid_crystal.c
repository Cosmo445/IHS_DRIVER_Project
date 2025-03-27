#include "liquid_crystal.h"
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include "../ioctl_cmds.h"

char state = WRITE;

void lcd_write_msg(char *msg) {
	
}

void lcd_write_char(char c) {
	
}


void lcd_write_info(int fd, unsigned int lcd_info) {

    ioctl(fd, WR_LCD);
    write(fd, &lcd_info, 4);  
    for(int i = 0; i < 30000; i++) {
    }  
    lcd_info = lcd_info & 0b11111111111111111111110111111111;
    write(fd, &lcd_info, 4);  

}

void lcd_init(int fd) {

    unsigned int lcd_info = 0;

    lcd_info = 0b0000000000000000001000001000111000;

    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0b0000000000000000001000001000001110;
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0b0000000000000000001000001000000001;
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0b0000000000000000001000001000000010;
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0b0000000000000000001000001000000110;
    lcd_write_info(fd, lcd_info);

    // Escreve H para teste    
    lcd_info = 0b0000000000000000001010001001001000;
    lcd_write_info(fd, lcd_info);

}
