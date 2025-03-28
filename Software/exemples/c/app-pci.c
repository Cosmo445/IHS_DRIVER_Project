#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc, atoi, rand... */
#include <string.h>	/* memcpy, strlen... */
#include <stdint.h>	/* uints types */
#include <sys/types.h>	/* size_t ,ssize_t, off_t... */
#include <unistd.h>	/* close() read() write() */
#include <fcntl.h>	/* open() */
#include <sys/ioctl.h>	/* ioctl() */
#include <errno.h>	/* error codes */

#include <time.h>

// ioctl commands defined for the pci driver header
#include "../../include/ioctl_cmds.h"

int main(int argc, char** argv)
{
	int fd, retval, PEDRO;

	if (argc < 2) {
		printf("Syntax: %s <device file path>\n", argv[0]);
		return -EINVAL;
	}

	if ((fd = open(argv[1], O_RDWR)) < 0) {
		fprintf(stderr, "Error opening file %s\n", argv[1]);
		return -EBUSY;
	}

	unsigned int data = 0x40404079;
	
	ioctl(fd, WR_R_DISPLAY);
	retval = write(fd, &data, sizeof(data));
	printf("wrote %d bytes\n", retval);
	
	ioctl(fd, RD_SWITCHES);
	read(fd, &data, 1);
	ioctl(fd, WR_RED_LEDS);
	retval = write(fd, &data, sizeof(data));
	printf("wrote %d bytes\n", retval);
	
	data = 0xFFFFFFFF;
	ioctl(fd, WR_GREEN_LEDS);
	retval = write(fd, &data, sizeof(data));
	printf("wrote %d bytes\n", retval);
	
	char verde = 1;
	
	while(1) {
		
		/*inicia frame*/int prev = clock();
		
		data = 0;

		//ESCREVE NOS HEX
		data = 0x0000FFFF;
		ioctl(fd, WR_R_DISPLAY);
		retval = write(fd, &data, sizeof(data));
		data = 0xFFFF0000;
		ioctl(fd, WR_L_DISPLAY);
		retval = write(fd, &data, sizeof(data));


		//LÊ DOS BOTÕES
		ioctl(fd, RD_PBUTTONS);
		read(fd, &data, 1);
		
		
		// LÊ DOS SWICHES
		ioctl(fd, RD_SWITCHES);
		read(fd, &data, 1);
		
		// PRINTA LEITURA NO TERMINAL
		printf("new data: 0x%X\n", data);
		for(int i = 1<<18; i > 0; i>>=1) {
			printf("%c", data & i ? '1':'0');
		}
		printf("\n");
		
		// ESCREVE LEITURA NOS LEDS
		ioctl(fd, WR_RED_LEDS);
		retval = write(fd, &data, sizeof(data));
		printf("wrote %d bytes\n", retval);
		
		// ANIMACAO DOS LEDS VERDES
		verde<<=1;
		if(verde==0)verde=1;
		ioctl(fd, WR_GREEN_LEDS);
		write(fd, &verde, sizeof(verde));
	
		/*termina frame*/while((clock()-prev) < CLOCKS_PER_SEC/12);
		
	}
	
	close(fd);
	return 0;
}