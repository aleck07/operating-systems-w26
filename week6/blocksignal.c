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
    sigset_t mask;
    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = SA_RESTART, // or SA_RESTART
    };
    sigemptyset(&mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    sigaddset(&mask ,SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    printf("SIGUSR1 is blocked\n");

    printf("Reading from the keyboard. But don't type anything.\n");
    printf("Go to another window and `kill -USR1 %d`\n", pid);
    printf("Then press RETURN to continue\n");

    char buffer[128];
    ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("read");
        exit(1);
    }

    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    printf("SIGNAL UNBLOCKED\n");

    return 0;
}
// The reason why we only see the one signal come through and get handled even if we kill it multiple times is because since they are the same type, they get collapsed into a single signal.