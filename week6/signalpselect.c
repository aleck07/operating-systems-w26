#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/select.h>

volatile sig_atomic_t sigusr1_happened;

void sigusr1_handler(int sig)
{
    (void)sig;
    sigusr1_happened = 1;
    // write(1, "SIGUSR1 handled\n", 16);
}

int main(void)
{
    pid_t pid = getpid();
    sigset_t zero;
    sigset_t mask;
    fd_set readfds;
    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = SA_RESTART, // or SA_RESTART
    };
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    sigemptyset(&zero);
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Either enter some text, or ...\n");
    printf("Go to another window and `kill -USR1 %d`\n", pid);
    printf("Then press RETURN to continue\n");

    int pselect_result = pselect(1, &readfds, NULL, NULL, NULL, &zero);
    if (pselect_result == -1 && errno == EINTR)
    {
        if (sigusr1_happened)
        {
            printf("pselect was interrupted by SIGUSR1\n");
        }
    }
    else if (pselect_result == -1)
    {
        perror("pselect");
        exit(1);
    }
    if (pselect_result > 0)
    {
        if (FD_ISSET(0, &readfds))
        {
            char buffer[128];
            ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0)
            {
                buffer[bytes_read] = '\0';
                puts(buffer);
            }
        }
    }

    return 0;
}