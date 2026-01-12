#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

int uppercaseBuf(char *buf, int len){
    for(int i = 0; i < len; i++){
        buf[i] = toupper(buf[i]);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // File descriptor
    int fd;
    char buf[2048];
    int bytes_read;

    fd = open(argv[1], O_RDONLY);

    if(argc == 1){
        while (true)
        {
            bytes_read = read(0, buf, sizeof(buf));
            uppercaseBuf(buf, sizeof(buf));
            write(1, buf, bytes_read);
        }
        
    }
    else {
        for(int i = 1; i <= argc; i++){
            fd = open(argv[i], O_RDONLY);
            while (true){
                bytes_read = read(fd, buf, sizeof(buf));
                uppercaseBuf(buf, sizeof(buf));
                write(1, buf, bytes_read);
                if(bytes_read < 2048){
                    break;
                }
            }
        }
    }

    close(fd);
}