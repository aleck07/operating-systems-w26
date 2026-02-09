#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int sig){
    (void)sig;
    write(1, "Got SIGINT!\n", 12);
}

int main(void)
{
    struct sigaction sa = {
        .sa_handler = sigint_handler,
        .sa_flags = 0, // or SA_RESTART
    };
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Program will exit in 10 seconds, hit ^C\n");
    sleep(10);

    return 0;
}