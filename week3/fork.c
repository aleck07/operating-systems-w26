#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("And...\n");

    pid_t pid = fork();

    if(pid == 0){
        printf("I'm the child\n");
    } else {
        printf("I'm the parent\n");
    }
    printf("We're done\n");

}
