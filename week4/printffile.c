#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void){
    int fd = open("./output.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644);
    
    // Error checking for file
    if (fd == -1) {
        perror("open");
        return 1;
    }

    dup2(fd, 1);

    printf("Hello, world\nThis is a test!\n");

    fflush(stdout);

    close(fd);
    return 0;
}