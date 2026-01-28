#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "./my_fifo"

int main(void)
{
    int fd;
    char buffer[128];
    ssize_t bytesRead;

    mkfifo(FIFO_NAME, 0644);

    fd = open(FIFO_NAME, O_RDONLY);
    printf("Consumer: Reading from FIFO...\n");
    while (1)
    {
        bytesRead = read(fd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0)
        {
            printf("Read: '%s'\n", buffer);
        }
    }
    close(fd);
    return 0;
}