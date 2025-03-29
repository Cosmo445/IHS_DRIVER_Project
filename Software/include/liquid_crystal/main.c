#include "liquid_crystal.h"
#include <sys/types.h>/* size_t ,ssize_t, off_t... */
#include <unistd.h>/* close() read() write() */
#include <fcntl.h>/* open() */
#include <sys/ioctl.h>/* ioctl() */
#include <errno.h>/* error codes */


int main(int argc, char **argv) {
    int fd, retval;

    if (argc < 2) {
        printf("Syntax: %s <device file path>\n", argv[0]);
        return -EINVAL;
    }

    if ((fd = open(argv[1], O_RDWR)) < 0) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return -EBUSY;
    }

	
	lcd_init(fd);
    lcd_write_msg(fd, "abcdefghijkl");
    return 0;

}
