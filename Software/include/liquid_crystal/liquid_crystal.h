#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include "../ioctl_cmds.h"

void lcd_write_msg(int, char *);
void lcd_write_char(int, char);
void lcd_write_control(int, char);
void lcd_drive_write(int, uint32_t);
void lcd_init(int);
void lcd_clear(int);

extern int update_data(char, char);
extern void pulse_enable(int, uint32_t);

#endif /* __LIQUID_CRYSTAL_H__ */
