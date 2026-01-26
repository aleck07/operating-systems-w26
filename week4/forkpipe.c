#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (void){
    int pfds[2]; // 0 is read, 1 is write.
    char buf[64] = {0};

    pipe(pfds);

    pid_t pid = fork();

    // Child process
    if (pid == 0){
        close(pfds[0]);
        write(pfds[1], "Hello, parent!\n", 15);
        exit(0);
    }

    close(pfds[1]);
    read(pfds[0], buf, sizeof buf);
    write(STDOUT_FILENO, buf, sizeof buf);

    wait(NULL);

}