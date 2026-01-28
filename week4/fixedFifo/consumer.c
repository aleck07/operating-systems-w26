#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIFO_NAME "./my_fifo"

int main(void)
{
    int fd;
    char buffer[128];
    int count;

    mkfifo(FIFO_NAME, 0644);

    fd = open(FIFO_NAME, O_RDONLY);

    read(fd, buffer, sizeof(buffer));
    count = atoi(&buffer[0]);
    for (int i = 0; i < count; i++)
    {
        read(fd, buffer, sizeof(buffer));
        printf("%d: Message #%d\n", buffer[0], buffer[0]);
    }

    close(fd);
    return 0;
}