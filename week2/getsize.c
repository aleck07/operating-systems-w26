#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    struct stat buf;

    if(argc == 1){
        stat(argv[1], &buf);
        printf("%s: %d\n", argv[1], buf.st_size);
    }
    else{
        for(int i = 1; i < argc; i++){
            stat(argv[i], &buf);
            printf("%s: %d\n", argv[i], buf.st_size);
        }
    }
}