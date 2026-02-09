#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigusr1_handler(int sig)
{
    (void)sig;
    write(1, "SIGUSR1 handled\n", 16);
}

int main(void)
{
    pid_t pid = getpid();
    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = 0, // or SA_RESTART
    };
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Reading from the keyboard. But don't type anything.\n");
    printf("Go to another window and `kill -USR1 %d`\n", pid);

    char buffer[128];
    ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("read");
        exit(1);
    }

    return 0;
}