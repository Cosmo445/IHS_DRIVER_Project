#include "liquid_crystal.h"
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include "../ioctl_cmds.h"

char state = WRITE;


void lcd_write_info(int fd, unsigned int lcd_info) {

    ioctl(fd, WR_LCD);
    write(fd, &lcd_info, sizeof(lcd_info));    
    lcd_info = lcd_info & 0b11110111111111;
    write(fd, &lcd_info, sizeof(lcd_info));  

}

void lcd_init(int fd) {

    unsigned int lcd_info = 0;

    lcd_info = 0x38 | (1 << 9) | (0 << 13);

    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0x0e | (1 << 9) | (0 << 13);
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0x01 | (1 << 9) | (0 << 13);
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0x02 | (1 << 9) | (0 << 13);
    lcd_write_info(fd, lcd_info);

    
    lcd_info = 0x06 | (1 << 9) | (0 << 13);
    lcd_write_info(fd, lcd_info);

    // Escreve H para teste    
    lcd_info = 0x48 | (1 << 9) | (1 << 13);
    lcd_write_info(fd, lcd_info);

}