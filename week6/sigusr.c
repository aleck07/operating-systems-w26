#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

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
        .sa_flags = SA_RESTART, // or SA_RESTART
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

// When we use SA_RESTART, our program doesn't close, it handles it and continues reading.