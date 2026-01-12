#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int, char *argv[]) {
    // File descriptor
    int fd;
    char buf[2048];

    fd = open(argv[1], O_RDONLY);

    int bytes_read = read(fd, buf, 2048);

    write(1, buf, bytes_read);

    close(fd);
}