#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void readDirectory(DIR *fd){
    struct dirent *dir;
    dir = readdir(fd);
    if(dir == NULL){
        return;
    }
    printf("%s\n", dir->d_name);
    readDirectory(fd);
}

int main(void){
    DIR *fd;
    fd = opendir("/");
    readDirectory(fd);
}

