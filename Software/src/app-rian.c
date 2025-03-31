#include <stdio.h>/* printf */
#include <stdlib.h>/* malloc, atoi, rand... */
#include <string.h>/* memcpy, strlen... */
#include <stdint.h>/* uints types */
#include <sys/types.h>/* size_t ,ssize_t, off_t... */
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include <errno.h>/* error codes */

#include <time.h>

// ioctl commands defined for the pci driver header
#include "../include/ioctl_cmds.h"

int main(int argc, char** argv)
{
    int fd, retval;

    if (argc < 2) {
        printf("Syntax: %s <device file path>\n", argv[0]);
        return -EINVAL;
    }

    if ((fd = open(argv[1], O_RDWR)) < 0) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return -EBUSY;
    }

    unsigned int data = 0x00000000;

    char verde = 1;
    
    int BATERIA = 1000, PRESSAO = 0, TEMPERATURA = 0;
    srand(0);
    int segm = 1;
    int map_7_seg[10] = {
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

    while(1) {
        /*inicia frame*/int prev = clock();
        
        if(PRESSAO<99)PRESSAO+=rand()%2;
        if(TEMPERATURA<99)TEMPERATURA+=rand()%2;
        if(BATERIA>0)BATERIA-=rand()%20;
        if(BATERIA<0)BATERIA=0;
        
        // ESCREVE LEITURA NOS 7SEG DA DIREITA
        /*segm<<=1; if(segm==0)segm=1;
        data=segm^0xFFFFFFFF;*/
        data = 
        	(map_7_seg[BATERIA/1000%10]<<24) +
        	(map_7_seg[BATERIA/100%10]<<16) +
        	(map_7_seg[BATERIA/10%10]<<8) +
        	(map_7_seg[BATERIA  %10])
        ;
        data = ~data;
        ioctl(fd, WR_R_DISPLAY);
        retval = write(fd, &data, sizeof(data));
        
        //ESCREVE LEITURA NOS 7SEG DA ESQUERDA
        data = 
        	(map_7_seg[TEMPERATURA/10]<<24) +
        	(map_7_seg[TEMPERATURA%10]<<16) +
        	(map_7_seg[PRESSAO/10]<<8) +
        	(map_7_seg[PRESSAO%10])
        ;
        data = ~data;
        ioctl(fd, WR_L_DISPLAY);
        retval = write(fd, &data, sizeof(data));

        // LÊ DOS SWICHES
        ioctl(fd, RD_SWITCHES);
        read(fd, &data, 4);

        // PRINTA LEITURA NO TERMINAL
        printf("new data: 0x%X\n", data);
        for(int i = 1<<17; i > 0; i>>=1) {
            printf("%c", data & i ? '1':'0');
        }
        printf("\n");
        
        // ESCREVE LEITURA NOS LEDS
        ioctl(fd, WR_RED_LEDS);
        retval = write(fd, &data, sizeof(data));

        // LÊ DOS BOTOES
        ioctl(fd, RD_PBUTTONS);
        read(fd, &data, 4);

        // ANIMACAO DOS LEDS VERDES
        //verde<<=1;
        //if(verde==0) verde=1;
        verde = rand();
        ioctl(fd, WR_GREEN_LEDS);
        write(fd, &verde, sizeof(verde));
        

        /*termina frame*/while((clock()-prev) < CLOCKS_PER_SEC/12);
    }

    close(fd);
    return 0;
}