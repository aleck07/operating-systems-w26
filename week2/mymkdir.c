#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("usage: mymakedir dir\n");
        return -1;
    } else {
        mkdir(argv[1], 0777);
        perror(argv[1]);
    }

}