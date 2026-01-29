#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Validate arg amount
    if (argc > 2)
    {
        printf("Invalid usage: lswc [directory]\n");
        exit(1);
    }
    if (argv == NULL)
    {
        argv[1] = ".";
    }

    int pfds[2];

    pipe(pfds);
    pid_t pid = fork();

    // Child Process
    if (pid == 0)
    {
        close(pfds[1]);
        dup2(pfds[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
        exit(0);
    }
    close(pfds[0]);
    dup2(pfds[1], STDOUT_FILENO);
    execlp("ls", "ls", argv[1], "-1a", NULL);

    wait(NULL);
    return 0;
}