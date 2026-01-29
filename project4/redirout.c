#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("usage: redirout filename command [arg1 [arg2 ... ] ]\n");
    }
    char *filename = argv[1];
    char *command = argv[2];
    char **args = &argv[2];

    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    execvp(command, args);
    return 0;
}