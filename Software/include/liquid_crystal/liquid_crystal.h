#ifndef __LIQUID_CRYSTAL_H__
#define __LIQUID_CRYSTAL_H__

#define WRITE = 1;
#define WAIT = 0;
extern char state;

void lcd_write(char *);
void lcd_write_info(int, unsigned int);
int lcd_init(int);

#endif /* __LIQUID_CRYSTAL_H__ */