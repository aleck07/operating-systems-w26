#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
    int pfds[2];  // pipe file descriptors

                // 0 is the read end, 1 is the write end

    // create the pipe
    pipe(pfds); 

    write(pfds[1], "Hello\n", 7); // write to the pipe
    char buf[7];
    read(pfds[0], buf, 7);   // read from the pipe
    write(STDOUT_FILENO, buf, 7); // write to standard output
}
