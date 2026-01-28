#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "./my_fifo"

int main(int argc, char *argv[])
{
    int fd;
    (void)argc;

    mkfifo(FIFO_NAME, 0644);
    fd = open(FIFO_NAME, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]) + 1);
    printf("%s" , argv[1]);
    close(fd);
    return 0;
}