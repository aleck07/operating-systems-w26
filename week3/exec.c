#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    printf("And...\n");

    pid_t pid = fork();

    if(pid == 0){
        execlp("ls", "ls", NULL);
    } else {
        printf("I'm the parent\n");
        printf("We're done\n");
        wait(NULL);
    }
}