#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

volatile sig_atomic_t sigusr1_happened;

void sigusr1_handler(int sig)
{
    (void)sig;
    sigusr1_happened = 1;
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

    printf("Go to another window and `kill -USR1 %d`\n", pid);

    while (!sigusr1_happened)
    {
        sleep(1);
    }
    // We add sleep in the loop so our program isn't constantly checking if the variable changed. It's heavily straining on the CPU.
    // The volatile keyword in C is used to indicate that the given variable may change unexpectidly, and force it to always be written to memory. 
    
    write(1, "SIGUSR1 handled\n", 16);

    return 0;
}
