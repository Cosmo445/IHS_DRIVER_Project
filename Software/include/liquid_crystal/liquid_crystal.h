#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

void lcd_write_msg(int, char *);
void lcd_write_char(int, char);
void lcd_write_control(int, char);
void lcd_drive_write(int, uint32_t);
void lcd_init(int);
void lcd_clear(int);

/* 
home
setCursor
noCursor
*/

extern int update_data(char, char);
extern void pulse_enable(int, uint32_t);

#endif /* __LIQUID_CRYSTAL_H__ */


