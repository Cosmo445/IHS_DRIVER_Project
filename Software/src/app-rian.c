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
#include "../../include/ioctl_cmds.h"

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

    while(1) {
        /*inicia frame*/int prev = clock();

        // LÊ DOS SWICHES
        ioctl(fd, RD_SWITCHES);
        read(fd, &data, 4);

        // PRINTA LEITURA NO TERMINAL
        printf("new data: 0x%X\n", data);
        for(int i = 1<<17; i > 0; i>>=1) {
            printf("%c", data & i ? '1':'0');
        }
        printf("\n");
        // ESCREVE LEITURA NOS 7SEG DA DIREITA
        ioctl(fd, WR_R_DISPLAY);
        retval = write(fd, &data, sizeof(data));
        // ESCREVE LEITURA NOS LEDS
        ioctl(fd, WR_RED_LEDS);
        retval = write(fd, &data, sizeof(data));

        // LÊ DOS BOTOES
        ioctl(fd, RD_PBUTTONS);
        read(fd, &data, 4);
        //ESCREVE LEITURA NOS 7SEG DA ESQUERDA
        ioctl(fd, WR_L_DISPLAY);
        retval = write(fd, &data, sizeof(data));

        // ANIMACAO DOS LEDS VERDES
        verde<<=1;
        if(verde==0) verde=1;
        ioctl(fd, WR_GREEN_LEDS);
        write(fd, &verde, sizeof(verde));

        /*termina frame*/while((clock()-prev) < CLOCKS_PER_SEC/12);
    }

    close(fd);
    return 0;
}