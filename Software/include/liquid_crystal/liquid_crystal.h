#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

void lcd_write_msg(char *);
void lcd_write_char(char);
void lcd_write_info(int, unsigned int);
void lcd_init(int);

#endif /* __LIQUID_CRYSTAL_H__ */

/* 
pulseEnable
init
clear
home
setCursor
write
noCursor
*/
