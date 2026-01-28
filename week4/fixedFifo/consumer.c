#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIFO_NAME "./my_fifo"
#define MSG_SIZE 16

int main(void)
{
    int fd;
    char buf[MSG_SIZE];
    ssize_t bytesRead;

    mkfifo(FIFO_NAME, 0644);

    fd = open(FIFO_NAME, O_RDONLY);
    printf("waiting for writers...\n");

    while (1)
    {
        bytesRead = read(fd, buf, MSG_SIZE);
        if (bytesRead > 0)
        {
            printf("%d: %s\n", (unsigned char)buf[0], buf + 1);
        }
        else
        {
            break;
        }
    }

    close(fd);
    return 0;
}