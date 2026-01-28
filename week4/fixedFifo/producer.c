#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define FIFO_NAME "./my_fifo"
#define MSG_SIZE 16

int main(int argc, char *argv[])
{
    (void)argc;
    int fd;
    char buf[MSG_SIZE] = {0};
    int count = atoi(argv[1]);

    if (count < 1 || count > 255)
    {
        printf("Count must be between 1 and 255\n");
        return 1;
    }

    mkfifo(FIFO_NAME, 0644);
    fd = open(FIFO_NAME, O_WRONLY);
    printf("waiting for readers...\n");
    for (int i = 0; i < count; i++)
    {
        buf[0] = i;                         // Set the message number
        sprintf(buf + 1, "Message #%d", i); // Set the message
        write(fd, buf, MSG_SIZE);
        printf("Sending %d Message #%d\n", i, i);
        sleep(1);
    }
    close(fd);
    return 0;
}