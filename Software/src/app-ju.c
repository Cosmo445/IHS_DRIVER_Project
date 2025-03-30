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

    unsigned int data = 0x00000000000000000000000000000000;

    while(1) {

        ioctl(fd, RD_SWITCHES);
        read(fd, &data, 4);

        ioctl(fd, WR_GPIO);
        retval = write(fd, &data, 4);
    }

    close(fd);
    return 0;
}
